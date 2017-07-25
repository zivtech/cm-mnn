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


/**
 *  Implements  hook_field_formatter_settings_form().
 */
function internet_archive_field_formatter_settings_form($field, $instance,
							$view_mode, $form,
							&$form_state) {
  //This gets the view_mode where our settings are stored
  $display = $instance['display'][$view_mode];
  //This gets the actual settings
  $settings = $display['settings'];
  //Initialize the element variable
  $element = array();

  //Add your select box
  if ($display['type'] == 'internet_archive_thumbnail' ||
      $display['type'] == 'internet_archive_video_embed') {
    $element['archive_links'] =
      array(
	    '#type' => 'select', // Use a select box widget
	    '#title' => t('Links?'), // Widget label
	    '#description' => t('Show Archive.org Links?'), // Helper text
	    '#default_value' => $settings['archive_links'], 
	    '#options' => array(
				'show' => 'Show',
				'hide' => 'Hide',
				),
	  );
  }
  if ($display['type'] == 'internet_archive_video_embed') {
    $element['archive_embed_width'] =
      array(
	    '#type' => 'textfield',
	    '#title' => t('Embed Width'),
	    '#size' => 4,
	    '#maxlength' => 4,
	    '#default_value' => isset($settings['archive_embed_width']) ?
	    $settings['archive_embed_width'] : 470,
	    );
    
    $element['archive_embed_height'] =
      array(
	    '#type' => 'textfield',
	    '#title' => t('Embed Height'),
	    '#size' => 4,
	    '#maxlength' => 4,
	    '#default_value' => isset($settings['archive_embed_height']) ?
	    $settings['archive_embed_height'] : 371,
	    );
  }
  return $element;
}
/**
 * Implements hook_field_formatter_view
 */
function internet_archive_field_formatter_view($entity_type, $entity, $field,
					       $instance, $langcode, $items,
					       $display) {
  
  $element = array();
  $html = '';
  
  $settings = $display['settings'];

  $show_links = FALSE;
  global $internet_archive_show_links;
  if (isset($settings['archive_links']) &&
      $settings['archive_links'] == 'show') {
    $internet_archive_show_links = TRUE;
  }
  else {
    $internet_archive_show_links = FALSE;
  }

  $width = isset($settings['archive_embed_width']) ?
    $settings['archive_embed_width'] : 470;

  $height = isset($settings['archive_embed_height']) ?
    $settings['archive_embed_height'] : 371;

  $fields = internet_archive_fields();
  foreach ($fields as $field_name => $field_info) {
    if ($field_name == $field['field_name'] && $items) {
      $archive_data = internet_archive_load_data_nid($entity->nid,$field_name);
      switch ($display['type']) {
      case 'internet_archive_video_embed':
	$html = internet_archive_embed_from_archive_data($archive_data,
							 $width, $height);
	break;
      case 'internet_archive_thumbnail':
	$thumb = internet_archive_thumb_from_archive_data($archive_data);
	$html = '<img src="' . $thumb . '" />';
	break;
      default:
	break;
      }

      $element[0]['#markup'] = $html;
    }
  }
  


  return $element;
}

