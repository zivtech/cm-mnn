<?php

/**
 * @file
 * Drupal Queue integration for Internet Archive Download Module.
 */

/**
 * Provides an interface to add items to the queue, to retrieve (claim)
 * an item from the head of the queue, and to claim and delete. Also
 * allows the user to run cron manually, so that claimed items can be
 * released.
 */
function internet_archive_download_queue_add_remove_form($form, &$form_state) {
  $queue_name = 'internet_archive_download_file';
  $items = internet_archive_download_queue_retrieve_queue($queue_name);

  // Add CSS to make the form a bit denser.
  $form['#attached']['css'] = array(drupal_get_path('module', 'internet_archive') . '/includes/internet_archive_queue.css');
  $form['internet_archive_download_queue_monitor'] = array(
    '#type' => 'fieldset',
    '#title' => 'Archive.org Download Queue Monitor',
    '#description' => '<div>' . t('This page is a manual interface to the Internet Archive file download queue. All files currently queued for download should be displayed in the queue status area below.') . '</div>',
  );

  $form['queue_name'] = array(
    '#type' => 'hidden',
    '#value' => $queue_name,
  );

  $form['run_drupal_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Download items in queue'),
    '#submit' => array('internet_archive_download_queue_drupal_queue_cron_run'),
  );
  $form['delete_queue'] = array(
    '#type' => 'submit',
    '#value' => t('Empty the Queue'),
    '#submit' => array('internet_archive_download_queue_add_remove_form_clear_queue'),
  );
  $form['status_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Queue status'),
    '#description' => t('Displays items in the queue to be downloaded from archive.org'),
    '#collapsible' => TRUE,
  );
  $form['status_fieldset']['status'] = array(
    '#type' => 'markup',
    '#value' => theme('download_queue_items', array('items' => $items)),
  );

  return $form;
}

/**
 * Submit handler for clearing/deleting the queue.
 */
function internet_archive_download_queue_add_remove_form_clear_queue($form, &$form_state) {

  drupal_queue_include();
  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $queue->deleteQueue();

  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM {internet_archive_download} WHERE status='queued'") */
  db_delete('internet_archive_download')
  ->condition('status', 'queued')
  ->execute();
  drupal_set_message(t('Deleted the @queue_name queue and all items in it', array('@queue_name' => $form_state['values']['queue_name'])));
}

/**
 * Retrieve the queue from the database for display purposes only.
 *
 * It is not recommended to access the database directly, and this is only here
 * so that the user interface can give a good idea of what's going on in the
 * queue.
 *
 * @param $queue_name
 *   The name of the queue from which to fetch items.
 */
function internet_archive_download_queue_retrieve_queue($queue_name) {
  $items = array();
  $result = db_query("SELECT item_id, data, expire, created FROM {queue} WHERE name = :name ORDER BY item_id", array(':name' => $queue_name));

  while ($item = db_fetch_array($result)) {
    $items[] = $item;
  }

  return $items;
}

/**
 * Copy of drupal_queue_cron_run limited to just the internet_archive_download queue.
 */
function internet_archive_download_queue_drupal_queue_cron_run() {
  drupal_queue_include();

  // Grab the defined cron queues.
  $queues = module_invoke_all('cron_queue_info');
  drupal_alter('cron_queue_info', $queues);

  // Work off queues.
  foreach ($queues as $queue_name => $info) {
    if ($queue_name == 'internet_archive_download_file') {
      $limit = 1;
      $counter = 0;
      $function = $info['worker callback'];
      $end = REQUEST_TIME + (isset($info['time']) ? $info['time'] : 15);
      $queue = DrupalQueue::get($queue_name);
      while (REQUEST_TIME < $end && ($item = $queue->claimItem()) && $counter < $limit) {
        $download = internet_archive_download_load_download($item->data);
        if ($download['status'] != 'downloaded' && $download['status'] != 'downloading') {
          $function($item->data);
          $queue->deleteItem($item);
          $counter++;
        }
      }
    }
  }
}
