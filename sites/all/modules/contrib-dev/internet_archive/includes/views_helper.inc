<?php

/**
 * Loads all views indicated as file views on the general settings page.
 *
 * @return $views
 *   An array of view names.
 */
function internet_archive_file_views() {
  $all_views = views_get_all_views();

  $views = array();
  foreach ($all_views as $key => $view) {
    if ($view->base_table == 'files' || $view->base_table == 'node') {
      $views[$view->name] = $view->name;
    }
  }
  asort($views);
  return $views;
}

/**
 * implements hook_action_info()
 */
function internet_archive_action_info() {
  $items = array();
    $items['internet_archive_delete_archive_item_action'] =
    array(
	  'type' => 'entity',
	  'label' =>
	  t('Internet Archive Transfers: Delete files, buckets, and items'),
	  'configurable' => FALSE,
	  'vbo_configurable' => FALSE,
	  'triggers' => array('any'),
	  'behavior' => array(),
	  );

  $items['internet_archive_complete_transfer_action'] =
    array(
	  'type' => 'entity',
	  'label' =>
	  t('Internet Archive Transfers: Validate and get Derivatives'),
	  'configurable' => FALSE,
	  'vbo_configurable' => FALSE,
	  'triggers' => array('any'),
	  'behavior' => array(),
	  );

  
  return $items;
}

/**
 * Custom VBO action callback function
 */
function internet_archive_complete_transfer_action(&$node, $context) {
  internet_archive_validate_transfers($node->nid);

  internet_archive_store_derivatives($node->nid);
}
/**
 * Custom VBO action callback function
 */
function internet_archive_delete_archive_item_action(&$node, $context){

  $sql = "SELECT * from {internet_archive} where nid = :nid";
  $results = db_query($sql, array(':nid' => $node->nid));

  while($result = $results->fetchAssoc()) {
    $archive_data = internet_archive_load_data_tid($result['tid']);
    internet_archive_delete_files($archive_data);
    internet_archive_delete_item($archive_data);

    db_delete('internet_archive')
      ->condition('tid', $archive_data['tid'])
      ->execute();

    $msg = t('Selected files at Archive.org deleted. <strong>Note that it '.
	     'may take some time for this update to be visible at '.
	     'Archive.org!</strong>');
	     
    drupal_set_message($msg);
    $log_entry = array(
      'tid' => $archive_data['tid'],
      'message' => t('File and derivatives deleted at Archive.org'),
      'message_data' => $archive_data,
      'type' => ARCHIVE_LOG_WARNING,
    );
    internet_archive_log($log_entry);

  }
  
}
/**
 * implements hook_form_alter()
 */
function internet_archive_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#id'] == 'views-exposed-form-internet-archive-transfers-page') {
    $options = array(
		     '' => '<select>',
		     ARCHIVE_QUEUED => ARCHIVE_QUEUED,
		     ARCHIVE_TRANSFERRING => ARCHIVE_TRANSFERRING,
		     ARCHIVE_TRANSFERRED => ARCHIVE_TRANSFERRED,
		     ARCHIVE_VALIDATED => ARCHIVE_VALIDATED,
		     ARCHIVE_DERIVED => ARCHIVE_DERIVED,
		     ARCHIVE_DELETED => ARCHIVE_DELETED,
		     ARCHIVE_FAILED => ARCHIVE_FAILED,
		     );

    $form['status_1'] =
      array(
	    '#type' => 'select',
	    '#options' => $options,
	  );


  }
}

