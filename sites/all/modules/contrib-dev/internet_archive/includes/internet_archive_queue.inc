<?php

/**
 * @file
 * Drupal Queue integration for Internet Archive Module.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_queue_settings_form($form, &$form_state) {
  $form['internet_archive_queue'] = array(
    '#type' => 'fieldset',
    '#title' => 'Archive.org Queue Management Settings',
    '#description' => t('The settings below provide basic automation of file transfers to Archive.org. These settings are in addition to any configurations you create in Media Mover, or crontab settings for your Drupal Queue. In other words, if you have configured your drupal queue crontab to run every 15 minutes, but selected only Sundays below -- files will be transferred every 15 minutes on Sundays only.') . '</div>',
  );
  if (module_exists('views')) {
    $form['internet_archive_queue']['internet_archive_file_views'] = array(
      '#type' => 'select',
      '#title' => 'Automatically add files to the queue from the following file views',
      '#default_value' => variable_get('internet_archive_file_views', FALSE),
      '#options' => internet_archive_file_views(),
      '#multiple' => TRUE,
      '#description' => t('This setting allows you to use views to automatically select files to add to the transfer queue. Each view selected should contain one of the fields specified on the <a href="/admin/settings/internet_archive">Main tab</a>. Node views should have row style set to fields, but most styles (unformatted, table, etc.) should work. Instead of Node, you can also use a File view type, just make sure to include the fid field in the display. In a standard configuration, files will be harvested when cron is run.'),
    );
    $form['internet_archive_queue']['internet_archive_queue_harvest_cron'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add files to queue on Drupal cron runs?'),
      '#description' => t('Checking this will cause the queue to be populated via the above views on Drupal cron runs. If this is not checked, you will need to either harvest files manually or configure Drush and use the ia-transfer command which includes the harvest from views step.'),
      '#default_value' => variable_get('internet_archive_queue_harvest_cron', FALSE),
    );
    $form['internet_archive_queue']['internet_archive_cron_user'] = array(
      '#type' => 'textfield',
      '#title' => t('Which user should queue processing be executed as?'),
      '#size' => 30,
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => variable_get('internet_archive_cron_user', FALSE),
      '#description' => t('You only need to set this if the views you are harvesting from above have data that is not accessible to anonymous users.'),
    );
  }

  $form['internet_archive_queue']['internet_archive_queue_attempts'] = array(
    '#type' => 'textfield',
    '#title' => 'If a file transfer fails, how many additional attempts should be made',
    '#default_value' => variable_get('internet_archive_queue_attempts', 0),
    '#description' => 'Transfers are relatively reliable to archive.org, but in some cases when traffic is heavy a file transfer may not complete. If this setting is not zero, the queue will attempt to retransfer failed files.',
    '#size' => 5,
  );
  $form['internet_archive_queue']['internet_archive_expired_queue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Requeue items that expire in the queue before transfer?'),
    '#description' => t('This is not generally necessary but may be helpful if you are transferring very large files and are seeing some files never start / stay on queued status for long periods of time.'),
    '#default_value' => variable_get('internet_archive_expired_queue', FALSE),
  );

  $form['internet_archive_queue']['internet_archive_queue_days'] = array(
    '#type' => 'select',
    '#title' => 'Select the day/s on which files should be transferred',
    '#default_value' => variable_get('internet_archive_queue_days', 'none'),
    '#description' => 'If no days are selected, transfers will any day that the queue cron is executed',
    '#options' => internet_archive_queue_day_options(),
    '#multiple' => TRUE,
  );
  $form['internet_archive_queue']['internet_archive_queue_start_time'] = array(
    '#type' => 'select',
    '#title' => 'Select the hour that transfers should start processing',
    '#default_value' => variable_get('internet_archive_queue_start_time', 'none'),
    '#options' => internet_archive_queue_hour_options(),
    '#multiple' => FALSE,
  );
  $form['internet_archive_queue']['internet_archive_queue_end_time'] = array(
    '#type' => 'select',
    '#title' => 'Select the hour that the transfers should stop processing',
    '#default_value' => variable_get('internet_archive_queue_end_time', 'none'),
    '#options' => internet_archive_queue_hour_options(),
    '#multiple' => FALSE,
  );
  $form['internet_archive_queue']['internet_archive_queue_limit'] = array(
    '#type' => 'textfield',
    '#title' => 'Enter the maximum number of items to send to archive.org in a single cron run',
    '#default_value' => variable_get('internet_archive_queue_limit', '1'),
    '#description' => t('This setting can be important if you have a slower internet connection, or are sending very large files to archive.org. Setting this to a small number can help prevent timeouts.'),
    '#size' => 5,
  );
  return system_settings_form($form);
}

/**
 * Provides an interface to add items to the queue, to retrieve (claim)
 * an item from the head of the queue, and to claim and delete. Also
 * allows the user to run cron manually, so that claimed items can be
 * released.
 */
function internet_archive_queue_add_remove_form($form, &$form_state) {
  $queue_name = 'internet_archive_send_file';
  $items = internet_archive_queue_retrieve_queue($queue_name);

  // Add CSS to make the form a bit denser.
  $form['#attached']['css'] = array(drupal_get_path('module', 'internet_archive') . '/includes/internet_archive_queue.css');
  $form['internet_archive_queue_monitor'] = array(
    '#type' => 'fieldset',
    '#title' => 'Archive.org Queue Monitor' . internet_archive_help_links('upload-queue-monitor'),
    '#description' => internet_archive_help_links('upload-queue-monitor', 'link'),
  );

  $form['queue_name'] = array(
    '#type' => 'hidden',
    '#value' => $queue_name,
  );

  $form['run_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Harvest files available for transfer'),
    '#submit' => array('internet_archive_queue_add_remove_form_run_cron'),
  );
  $form['run_drupal_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Transfer items in queue'),
    '#submit' => array('internet_archive_queue_drupal_queue_cron_run'),
  );
  $form['delete_queue'] = array(
    '#type' => 'submit',
    '#value' => t('Empty the Queue'),
    '#submit' => array('internet_archive_queue_add_remove_form_clear_queue'),
  );
  $form['status_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Queue status'),
    '#description' => t('Displays items in the queue to be transfered to archive.org'),
    '#collapsible' => TRUE,
  );
  $form['status_fieldset']['status'] = array(
    '#type' => 'markup',
    '#value' => theme('queue_items', array('items' => $items)),
  );

  return $form;
}

/**
 * Submit function for the insert-into-queue button.
 */
function internet_archive_queue_add_remove_form_insert($form, &$form_state) {
  // Get a queue (of the default type) called 'internet_archive_queue_queue'.
  // If the default queue class is SystemQueue this creates a queue that stores
  // its items in the database.
  drupal_queue_include();
  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $queue->createQueue(); // There is no harm in trying to recreate existing.

  // Queue the string.
  $queue->createItem($form_state['values']['fid_to_add']);
  $count = $queue->numberOfItems();
  drupal_set_message(t('Queued your file (@fid_to_add). There are now @count items in the queue.', array('@count' => $count, '@fid_to_add' => $form_state['values']['fid_to_add'])));
  $form_state['rebuild'] = TRUE; // Allows us to keep information in $form_state.
  // Unsetting the string_to_add allows us to set the incremented default value
  // for the user so they don't have to type anything.
  unset($form_state['input']['fid_to_add']);
}

/**
 * Submit function for "run cron" button.
 *
 * Runs cron (to release expired claims) and reports the results.
 */
function internet_archive_queue_add_remove_form_run_cron($form, &$form_state) {
  internet_archive_harvest_files();
  drupal_queue_include();
  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $queue->createQueue(); // There is no harm in trying to recreate existing.
  $count = $queue->numberOfItems();
  drupal_set_message(t('Attempted to harvest files. There are now @count items in the queue.', array('@count' => $count)));
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for clearing/deleting the queue.
 */
function internet_archive_queue_add_remove_form_clear_queue($form, &$form_state) {
  internet_archive_queue_clear_item_status($form_state['values']['queue_name']);

  drupal_queue_include();
  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $queue->deleteQueue();

  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM {internet_archive} WHERE status='queued'") */
  db_delete('internet_archive')
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
function internet_archive_queue_retrieve_queue($queue_name) {
  $items = array();
  $result = db_query("SELECT item_id, data, expire, created FROM {queue} WHERE name = :name ORDER BY item_id", array(':name' => $queue_name));

  while ($item = db_fetch_array($result)) {
    $items[] = $item;
  }

  return $items;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_queue_clear_item_status($queue_name) {
  $items = internet_archive_queue_retrieve_queue($queue_name);
  foreach ($items as $item) {
    if ($item['data']) {
      $fid = unserialize($item['data']);
      if (is_numeric($fid)) {
        internet_archive_update_file_data($fid, array('status' => NULL));
      }
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_queue_hour_options() {
  $hours = array(
    'none' => 'Transfer whenever drupal_queue\'s cron is run',
    '0' => '0:00 (Midnight)',
    '1' => '1:00 (1:00am)',
    '2' => '2:00 (2:00am)',
    '3' => '3:00 (3:00am)',
    '4' => '4:00 (4:00am)',
    '5' => '5:00 (5:00am)',
    '6' => '6:00 (6:00am)',
    '7' => '7:00 (7:00am)',
    '8' => '8:00 (8:00am)',
    '9' => '9:00 (9:00am)',
    '10' => '10:00 (10:00am)',
    '11' => '11:00 (11:00am)',
    '12' => '12:00 (Noon)',
    '13' => '13:00 (1:00pm)',
    '14' => '14:00 (2:00pm)',
    '15' => '15:00 (3:00pm)',
    '16' => '16:00 (4:00pm)',
    '17' => '17:00 (5:00pm)',
    '18' => '18:00 (6:00pm)',
    '19' => '19:00 (7:00pm)',
    '20' => '20:00 (8:00pm)',
    '21' => '21:00 (9:00pm)',
    '22' => '22:00 (10:00pm)',
    '23' => '23:00 (11:00pm)',
  );

  return $hours;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_queue_day_options() {
  $days = array(
    'monday' => 'Monday',
    'tuesday' => 'Tuesday',
    'wednesday' => 'Wednesday',
    'thursday' => 'Thursday',
    'friday' => 'Friday',
    'saturday' => 'Saturday',
    'sunday' => 'Sunday',
  );

  return $days;
}

/**
 * Copy of drupal_queue_cron_run limited to just the internet_archive queue.
 *
 */
function internet_archive_queue_drupal_queue_cron_run() {
  drupal_queue_include();

  if (!internet_archive_hour_check() || !internet_archive_day_check()) {
    //setting the time range to negative causes the queue to skip execution
    $time = -60;
  }
  else {
    $time = 10800;
  }
  $lease = 9000; //Seconds allowed per transfer task
  $queue_name = 'internet_archive_send_file';

  $function = 'internet_archive_transfer_file_worker';
  $end = REQUEST_TIME + $time;
  $queue = DrupalQueue::get($queue_name);
  while (REQUEST_TIME < $end && ($item = $queue->claimItem($lease))) {
    $status = internet_archive_get_status($item->data);
    $archive_info = internet_archive_load_data($item->data);

    if ($status != ARCHIVE_TRANSFERRING) {
      //update the file status to transferring
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query("UPDATE {internet_archive} SET status='%s' WHERE in_path='%s'", ARCHIVE_TRANSFERRING, $item->data) */
      db_update('internet_archive')
  ->fields(array(
        'status' => ARCHIVE_TRANSFERRING,
      ))
  ->condition('in_path', $item->data)
  ->execute();
      $archive_info['status'] = ARCHIVE_TRANSFERRING;
      internet_archive_invoke_internet_archive($archive_info, 'transferring');

      $function($item->data);
      $queue->deleteItem($item);
    }
  }
}
