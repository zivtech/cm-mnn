<?php

/**
 * Implements hook_node_update().
 */
function internet_archive_node_update($node) {
  if (variable_get('internet_archive_update', FALSE)) {
    if ($files = internet_archive_node_files($node->nid)) {
      foreach ($files as $key => $archive_data) {
        internet_archive_update_item($node, $archive_data);
      }
    }
  }
}

/**
 * Implements hook_field_display_alter().
 * Adds archive.org file status and manual transfer links
 * beneath relevant field displays
 */
function internet_archive_field_attach_view_alter(&$output, $context) {
  //grab all applicable fields
  $fields = internet_archive_fields();

  foreach ($fields as $field_name => $field_info) {
    //get the value key to reference
    switch ($field_info['type']) {
    case 'file':
      $value_key = 'uri';
      break;
    case 'text':
      $value_key = 'value';
      break;
    default: 
      $value_key = NULL;
      drupal_set_message('Only the file field type is defined at this time',
			 'warning');
      break;
      
    }

    //IF OUR Archive.org FIELD HAS A VAULE, ADD A SUFFIX
    if (isset($output[$field_name]) && $output[$field_name]) {
      if ($output[$field_name]['#items'][0][$value_key]) {
        $suffix =
	  internet_archive_theme_field_display($field_name,
					       $output[$field_name]['#items'],
					       $value_key);
	$output[$field_name][0]['#suffix'] = $suffix;
      }
    }
  }
}

/**
 * Themes the additional archive.org field data provided on node view.
 * TODO: make into a real theme function
 *
 * @param $filefield
 *   A field array.
 * @param $value_key
 *   The name of the array value containing the file_name.
 *
 * @return $filefield_output
 *   A string containing formatted archive.org utility links
 */
function internet_archive_theme_field_display($field_name, $field, $value_key){
  $html = '';
  //loop through all of the data pairs on the field looking for the file
  foreach ($field as $file_info) {
    //did we find the file name we wanted?
    if ($file_info[$value_key]) {
      //grab the data from the internet archive using the file_name 
      $archive_data = internet_archive_load_data($file_info[$value_key],
						 $field_name);
      
      $path = 'https://www.archive.org/details/' .$archive_data['item'];
      
      //if our file has already been transferred and is in a positive state
      //we will give links to view the item on archive.org or download it
      if ($archive_data && ($archive_data['status'] == ARCHIVE_VALIDATED ||
			    $archive_data['status'] == ARCHIVE_DERIVED)) {
	$html .= "<li>";
        $html .=l(t("View Item on Archive.org"), $path);
	$html .= "</li><li>";
	$html .=l(t("Download from Archive.org"),$archive_data['archive_url']);
	$html .= "</li>";
	$html .= "<li>";
	$html .= l(t("Delete Files on Archive.org"),
		   "node/". $archive_data['nid'] ."/ia/delete-files/" .
		   $archive_data['tid']);
	$html .= "</li>";
	
      }
      //if we have data from teh internet archive table and we are queued
      //offer a link to see it in the internet_archive_send_file queue 
      else if ($archive_data && ($archive_data['status'] == ARCHIVE_QUEUED)) {
	$html .= "<li>";
	$html .= l(t("Transfer from Queue to Archive.org"),
		   "/ia/transfer/" . $archive_data['tid']);
	$html .= "</li>";

	$html .= "<li>";
	$html .= l(t("View item in the queue"),
		   "/admin/structure/internet_archive");
	$html .= "</li>";

      }
      //if we have data from teh internet archive table but we aren't yet
      //in a completed state, give a link to the item on archive.org
      //if it exists there it means we haven't run cron or use the transfer VBO
      elseif ($archive_data &&
	      ($archive_data['status'] == ARCHIVE_TRANSFERRED ||
	       $archive_data['status'] == ARCHIVE_TRANSFERRING)) {
	$html .= "<li>";
	$html .= l(t("Recheck Status for Incomplete Upload."),
		   "/ia/validate/" . $archive_data['nid']);
	$html .= "</li>";
      }
      //if we have data from teh internet archive table but we aren't yet
      //in a completed state, give a link to the item on archive.org
      //if it exists there it means we haven't run cron or use the transfer VBO
      elseif ($archive_data && ($archive_data['status'] != ARCHIVE_VALIDATED ||
				$archive_data['status'] != ARCHIVE_DERIVED)) {
	$html .= "<li>";
	$html .= l(t("View Partially Transferred Item on Archive.org"), $path);
	$html .= "</li>";

      }
      //we haven't started the transfer yet, so offer a link to do so
      else {
	global $archive_fields_info;
	$file_location =
	  isset($archive_fields_info[ARCHIVE_FILE_LOCATION_REMOTE]
		 [$field_name]) ?
	  ARCHIVE_FILE_LOCATION_REMOTE : ARCHIVE_FILE_LOCATION_LOCAL;

	$uri = $file_info[$value_key];
	$uri = urlencode(str_replace('/', '!-!', $uri)); 
	$transfer_path = 'internet_archive/' . $uri . '/transfer';
	$args = array('query' => drupal_get_destination());
	$args['query']['field_name'] = $field_name;
	$args['query']['file_location'] = $file_location;
	$html .= "<li>";
        $html .= l(t("Transfer File to Archive.org"), $transfer_path, $args);
	$html .= "</li>";
      }
    }
  }

  $html = '<span class="internet-archive-files"><ul>'.$html.'</ul></span>';

  return $html;
}

/**
 * Grabs all internet archive transfer information based on node id.
 *
 * @param $nid
 *   Node id
 *
 * @return
 *   An array of internet archive files associated with the node.
 */
function internet_archive_node_files($nid) {
  $result = db_query("SELECT * FROM {internet_archive} WHERE nid = :nid",
		     array(':nid' => $nid));
  $files = array();
  while ($archive_data = $result->fetchAssoc()) {
    $files[] = $archive_data;
  }

  return $files;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_node_status($node) {
  global $user;
  $files = internet_archive_node_files($node->nid);

  if (count($files) >= 1) {
    if (user_access('update all internet_archive metadata') ||
	(user_access('update own internet_archive metadata') &&
	 $user->uid == $node->uid) ||
	user_access('administer internet_archive')) {

      $operations .=
	l('Click here to update Metadata stored at Archive.org for ' .
	  'this node', 'node/' . $node->nid . '/ia/update-metadata');
    }
    if ($operations) {
      $output .= '<div class="admin-panel">';
      $output .= '<br />';
      $output .= '<h3>Available Operations:</h3>';
      $output .= $operations;
      $output .= '<br /><br />';
      $output .= '</div>';
    }
  }

  if (count($files) >= 1) {
    $output .= '<div class="admin-panel">';
    $output .= '<h3>Files currently at Archive.org:</h3>';
    $output .= internet_archive_node_files_table($node);
    $output .= '</div>';

    if (user_access('access internet_archive logs')) {
      $output .= '<div class="admin-panel">';
      $output .= internet_archive_log_view('nid', $node->nid);
      $output .= '</div>';
    }
  }

  if (!$output) {
    $output .=
      'There is currently no data stored at archive.org for this node.';
  }

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_node_files_table($node) {
  global $user;
  $files = internet_archive_node_files($node->nid);

  if (count($files >= 1)) {
    $header = array(t('File Name'), t('Archive Page'), t('Status'));
    if (user_access('delete all internet_archive files') ||
	(user_access('delete own internet_archive files') &&
	 $user->uid == $node->uid)) {
      $header[] = t('Delete files at Archive.org');
    }
    if (user_access('access internet_archive logs')) {
      $header[] = t('View log');
    }
    if (module_exists('internet_archive_download') &&
	user_access('administer internet_archive')) {
      $header[] = t('Re-Queue Download');
    }

    foreach ($files as $key => $archive_data) {
      unset($row);
      if (strlen($archive_data['file_name']) > 20) {
        $file_name = substr($archive_data['file_name'], 0, 20) . '...';
      }
      else {
        $file_name = $archive_data['file_name'];
      }
      $row = array(
        $file_name,
        l('Visit', 'http://www.archive.org/details/' . $archive_data['item']),
        $archive_data['status'],
      );
      if (user_access('delete all internet_archive files') ||
	  (user_access('delete own internet_archive files') &&
	   $user->uid == $node->uid)) {
        $row[] = l('Delete', 'node/' . $node->nid . '/ia/delete-files/' .
		   $archive_data['tid']);
      }
      if (user_access('access internet_archive logs')) {
        $row[] = l('Log', 'ia/log/' . $archive_data['tid']);
      }
      if (module_exists('internet_archive_download') &&
	  user_access('administer internet_archive')) {
        if ($archive_data['status'] == 'derived') {
          $row[] = l('Queue', 'ia/queue-download/' . $archive_data['tid']);
        }
      }

      $rows[] = $row;
    }

    return theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    return t('No files from this node have been transferred to Archive.org');
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_node_fields($node) {
  $fields = internet_archive_fields();
  $node_fields = array();
  if ($fields) {
    foreach ($fields as $field_name => $info) {
      if ($node->{$field_name}) {
        $node_fields[] = $field_name;
      }
    }
  }

  if (count($node_fields >= 1)) {
    return $node_fields;
  }
  else {
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_node_field_files($node) {
  //grab all applicable fields
  $fields = internet_archive_fields();

  $field_files = array();

  foreach ($fields as $field_name => $field_info) {
    //get the value key to reference
    switch ($field_info['type']) {
    case 'filefield':
      $value_key = 'file_name';
      break;
    case 'text':
    case 'emvideo':
      $value_key = 'value';
      break;
    case 'file':
      $value_key = 'uri';
      break;
    }

    if ($node->$field_name) {
      if ($node->{$field_name}[0][$value_key]) {
        $field_files[] = array(
          'field_name' => $field_name,
          'value' => $node->{$field_name}[0][$value_key],
        );
      }
    }
  }

  if (count($field_files >= 1)) {
    return $field_files;
  }
  else {
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_update_node_metadata($node) {
  if ($files = internet_archive_node_files($node->nid)) {
    foreach ($files as $key => $archive_data) {
      internet_archive_update_item($node, $archive_data);
    }
    drupal_set_message('Metadata at Archive.org updated. <strong>Note '.
		       'that it may take some time for this update to be '.
		       'visible at Archive.org!</strong>');
    drupal_goto('node/' . $node->nid . '/ia');
  }
  else {
    drupal_set_message('No item found for this node at Archive.org!');
    drupal_goto('node/' . $node->nid . '/ia');
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_delete_node_files($node, $tid) {
  if ($archive_data = internet_archive_load_data_tid($tid)) {
    internet_archive_delete_files($archive_data);

    db_delete('internet_archive')
      ->condition('tid', $tid)
      ->execute();
    $msg = t('Selected files at Archive.org deleted. <strong>Note that it '.
	     'may take some time for this update to be visible at '.
	     'Archive.org!</strong>');
	     
    drupal_set_message($msg);
    $log_entry = array(
      'tid' => $tid,
      'message' => t('File and derivatives deleted at Archive.org'),
      'message_data' => $archive_data,
      'type' => ARCHIVE_LOG_WARNING,
    );
    internet_archive_log($log_entry);
    drupal_goto('node/' . $node->nid);
  }
  else {
    drupal_set_message('No files found for this source at Archive.org!');
    $log_entry = array(
      'tid' => $tid,
      'message' => t('No entries found for this file at Archive.org'),
      'message_data' => $archive_data,
      'type' => ARCHIVE_LOG_ERROR,
    );
    internet_archive_log($log_entry);
    drupal_goto('node/' . $node->nid);
  }
}


/**
 * Finds a node connected to a file_name based on the fields chosen
 * in the general settings.
 *
 * @param $file_name
 *   A string containing a file path.
 *
 * @return $node
 *   Drupal node object, or FALSE if nothing is found.
 */
function internet_archive_get_node_from_file_name($file_name,
						 $field_name) {

  //FETCH AN ASSOCIATIVE ARRAY OF Archive.org FIELD DB TABLE INFO
  $tables = internet_archive_get_field_tables($field_name);

  //BUILD AN INDEX TO LOOK FOR OUR FIELD IN $table_info
  $index = 'field_data_' . $field_name;
  $table_info = isset($tables[$index]) ? $tables[$index] : NULL;

  if ($table_info) {
    switch ($table_info['type']) {
    case 'file':
      $query =
	"SELECT entity_id FROM {" . $table_info['name'] . "} ft ".
	"INNER JOIN {file_managed} fm " .
	"ON ft." .$table_info['value_column'] .	" = fm.fid ".
	"WHERE fm.uri=:uri";
      
      $nid = db_query($query, array(':uri' => $file_name))->fetchField();
      
      if ($nid) {
	$node = node_load($nid);
	return $node;
      }
      break;
    case 'text':
      $query =
	"SELECT entity_id FROM {" . $table_info['name'] . "} ".
	"WHERE " . $table_info['value_column'] . "=:file_name";
      
      $nid = db_query($query, array(':file_name' => $file_name))->fetchField();
      
      if ($nid) {
	$node = node_load($nid);
	return $node;
      }
      break;
    default:
      drupal_set_message('Only the file field type is defined at this time.',
			 'warning');
      return NULL;
      break;
    }
  }

  return FALSE;
}


/**
 * This function is called by the /ia/transfer_item menu hook and will
 * allow a item to be taken out of the queue and transferred
 */
function internet_archive_validate_and_derive($nid) {
  if (!$nid) {
    return;
  }

  internet_archive_validate_transfers($nid);
  internet_archive_store_derivatives($nid);
  drupal_goto('node/'. $nid);
}
