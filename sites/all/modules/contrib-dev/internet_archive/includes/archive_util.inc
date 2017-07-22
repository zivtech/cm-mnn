<?php

/**
 * Loads all fields chosen on the general settings page
 *
 * @return $fields
 *   An array of field objects.
 */
function internet_archive_fields() {
  global $archive_fields_info;

  $archive_fields_info = array();
  $archive_fields_info[ARCHIVE_FILE_LOCATION_LOCAL] = array();
  $archive_fields_info[ARCHIVE_FILE_LOCATION_REMOTE] = array();
  
  $fields = array();
  $field_info = field_info_field_map();
  if (($field_types = variable_get('internet_archive_fields', FALSE)) &&
      variable_get('internet_archive_fields_list_display', FALSE)) {
    foreach ($field_types as $field_name => $description) {
      $fields[$field_name] = $field_info[$field_name];
      $archive_fields_info[ARCHIVE_FILE_LOCATION_LOCAL][$field_name] =
	$field_info[$field_name];
    }
  }
  if (($field_types = variable_get('internet_archive_fields_remote', FALSE)) &&
      variable_get('internet_archive_fields_list_display', FALSE)) {
    foreach ($field_types as $field_name => $description) {
      $fields[$field_name] = $field_info[$field_name];
      $archive_fields_info[ARCHIVE_FILE_LOCATION_REMOTE][$field_name] =
	$field_info[$field_name];
    }
  }

  return $fields;
}


/**
 * Instantiates a new S3 class.
 *
 * @param $key
 *   A string containing an archive.org user account access key.
 * @param $skey
 *   A string containing an archive.org user account private key.
 *
 * @return $s3
 *   An s3 class provided by includes/archive.php.
 */
function internet_archive_new_s3($key = FALSE, $skey = FALSE) {
  // if keys are being passed in, override defaults
  // AWS access info
  if (!defined('archiveAccessKey')) {
    define('archiveAccessKey', $key ? $key :
	   variable_get('internet_archive_key', NULL));
  }
  if (!defined('archiveSecretKey')) {
    define('archiveSecretKey', $skey ? $skey :
	   variable_get('internet_archive_skey', NULL));
  }
  $s3 = new S3(archiveAccessKey, archiveSecretKey);

  return $s3;
}

/**
 * Tests the basic archive.org S3 settings from the general settings page by
 * attempting to create the default bucket at archive.org.
 *
 * @param $element
 * @param &$form_state
 *
 */
function internet_archive_admin_validate($element, &$form_state) {
  // only validate if we have both values
  if ($form_state['values']['internet_archive_key'] &&
      $form_state['values']['internet_archive_skey']) {

    $s3 =internet_archive_new_s3($form_state['values']['internet_archive_key'],
				 $form_state['values']['internet_archive_skey']
				 );

    // To test the configuration credentials, attempt to create default bucket.
    if (! $s3->putBucket($form_state['values']['internet_archive_bucket'],NULL,
		     $form_state['values']['internet_archive_default_perm'])) {
      $msg =
	t('Could not create your default item on the Archive servers. '.
	       'This most likely means that either:') . '<br />' .
	t('1) your default item name conflicts with one that already exists, '.
	  'or') . '<br />' .
	t('2) the access or secret key provided is incorrect.');
      form_error($element, $msg);
    }
    else {
      $msg = t('Connection to Archive.org tested successfully with the ' .
	       'credentials provided below.');
      drupal_set_message($msg);
    }
  }
}

/**
 * Sends a file to archive.org
 * TODO: improve documentation below
 * TODO: remove the extraneous debug watchdogs
 * TODO: find a better solution to the sleep hack
 *
 * @param $config
 *   An array containing s3 parameters
 *
 * @param $file_info
 *   An array of file_name, field_name, and file_location
 *
 * @return $path
 *   A string containing the itemname/file_name
 */
function internet_archive_send($config, $file_info) {
  dsm('internet_archive_send');
  $file_name = $file_info['file_name'];
  $field_name = $file_info['field_name'];
  $file_location = $file_info['file_location'];
 
  //grab any existing information available for this file_name
  $archive_data = internet_archive_load_data($file_name, $field_name);
  if ($archive_data && isset($archive_data['nid'])) {
    $node = node_load($archive_data['nid']);
  }
  if (!isset($node)) {
    $node = internet_archive_get_node_from_file_name($file_name, $field_name);
  }
  
  //DEBUG LOG
  if (variable_get('internet_archive_debug', FALSE)) {
    watchdog('internet_archive', 'Sending file: ' . $file_name, NULL,
	     WATCHDOG_NOTICE);
  }

  // load the S3 class
  if (! $s3 = internet_archive_new_s3()) {
    return FALSE;
  }

  if ($archive_data) {
    dsm($archive_data, 'found archive data');
    $s3->setTransferId($archive_data['tid']);
    //BUMP UP OUR NUMBER OF ATTEMPTS
    $archive_data['attempts'] ++;
  }
  else {
    $archive_data = array(
      'item' => $config['internet_archive_bucket'],
      'in_path' => $file_name,
      'file_name' => basename($file_name),
      'attempts' => 1,
      'field_name' => $field_name,
      'file_location' => $file_location,
      'md5' => internet_archive_get_md5_for_file($file_name, $file_location),
      'date' => REQUEST_TIME,
      'archive_url' => 'http://www.archive.org/download/' .
      $config['internet_archive_bucket'] . '/' . basename($file_name),
      'nid' => $node ? $node->nid : NULL,
    );
    dsm($archive_data, 'initialized archive data');
  }

  // IF THE FILE IS LOCAL AND WE CANT READ IT RETURN FALSE
  if ($file_location == ARCHIVE_FILE_LOCATION_LOCAL &&
      !is_readable($file_name)) {
    watchdog('internet_archive', 'File is not readable: ' . $file_name .
	     ', please check that this file exists and the web user has ' .
	     'read permissions to it.', NULL, WATCHDOG_ERROR);
    return FALSE;
  }
  // IF WE ARE LOCAL GET FILE SIZE
  else if ($file_location == ARCHIVE_FILE_LOCATION_LOCAL) {
    $file_size = internet_archive_file_size($file_name); 
  }
  else {
    // TODO, FIND WAY TO READ REMOTE FILE
  }

  //SET FILE INFO IF WE GOT IT
  $archive_data['file_size'] = isset($file_size) ? $file_size : 0;
  $mimetype = file_get_mimetype($file_name);
    
  //HACK for DOM, need to find a more general solution
  $pathinfo = pathinfo($file_name);
  if ($pathinfo['extension'] == 'mp2') {
    $mimetype = 'video/mpeg';
  }

  //grab metadata if it is available
  $metadata = array();
  $metadata = internet_archive_get_metadata($node, $file_name);
  dsm($metadata, "back from internet_archive_get_metadata");
  //TODO: add some sort of backup in case there is no title/pattern...

  //CHECK TO SEE THE METADATA HOOK HAS SET A BUCKET NAME, THIS TAKES
  //PRECEDENT OVER THE OTHER METHODS
  if (isset($metadata['bucket-name']) && $metadata['bucket-name']) {
    $name = $metadata['bucket-name'];
    unset($metadata['bucket-name']);
  }
  else if ($itempattern = variable_get('internet_archive_itemname', NULL)) {
    //FIXME, TOKENS ARE NOT WORKING RIGHT NOW
    $name = token_replace($itempattern, $type = 'node', $object = $node,
			  $leading = '[', $trailing = ']', $options = array());

    $name = internet_archive_create_item_name($name);
  }
  elseif (variable_get('internet_archive_bucket_title', FALSE)) {
    $name = internet_archive_create_item_name($metadata['title']);
  }
  else {
    $default_item =variable_get('internet_archive_bucket',
				variable_get('site_name', t('My Site')));
    
    $name = internet_archive_create_item_name($default_item);
  }

  if (variable_get('internet_archive_queue_attempts', 0) > 0) {
    if (isset($archive_data['status']) &&
	$archive_data['status'] == ARCHIVE_FAILED) {
      $name = $archive_data['item'];
    }
  }

  $headers = internet_archive_process_metadata($metadata);

  $config['internet_archive_bucket'] = $name;
  $s3->bucket = $config['internet_archive_bucket'];
  $archive_data['item'] = $config['internet_archive_bucket'];
  $archive_data['archive_url'] = 'http://www.archive.org/download/' .
    $config['internet_archive_bucket'] . '/' . basename($file_name);
  
  dsm($headers, "Putting up Bucket: $name");

  $status =
    $s3->putBucket($config['internet_archive_bucket'], $headers);

  //Log start of transfer time if we already have row in internet_archive
  $archive_data['transfer_initiated'] = REQUEST_TIME;

  //SET VARIABLES FOR THE WAIT LOOP ON BUCKET CREATTION
  //find a pause time for each loop, ideally so we get 3 nicely timed loops
  $pause = 5;
  $max = 100;
  $index = 0;
  $bucket_exists = FALSE;
  $start = strtotime('now');
  $max_run_time = 35;
    
  // There is a delay on archive.org regarding bucket creation.. so we check
  // every x seconds
  $pause = 5;
  while (!$bucket_exists && ($max_run_time + $start) > strtotime('now')){
    sleep($pause);
    if ($index > $max) {
      break;
    }
    $index++;
    if (internet_archive_bucket_exists($config['internet_archive_bucket'])) {
      $bucket_exists = TRUE;
    }
  }

  if (!$bucket_exists) {
    $archive_data['status'] = ARCHIVE_FAILED;
    if ($archive_data['tid']) {
      internet_archive_update_archive_data($archive_data);
    }
    else {
      $archive_data['tid'] =
	internet_archive_insert_archive_data($archive_data);
    }

    $msg = t("Error Code #23244 Bucket could not be found for !file_name, ".
	     "maybe bump up execution time from !max.",
	     array('!file_name' => $file_name,
		   '!max' => $max_run_time));
    
    watchdog('internet_archive', $msg, NULL, WATCHDOG_ERROR);
    drupal_set_message($msg, 'error');
    return FALSE;
  }

  dsm('found bucket!!!!!!!');
  
  //WE FOUND OUR BUCKET SO NOW ITS TIME TO UPLOAD FILE, EITHER FROM A LOCAL
  //SOURCE OR A REMOTE ONE.
  if (module_exists('internet_archive_remote') &&
      $file_location == ARCHIVE_FILE_LOCATION_REMOTE) {
    dsm("pushing remote file");
    $status =
      internet_archive_remote_put_object($file_name,
					 $config['internet_archive_bucket'],
					 basename($file_name), $headers,
					 $mimetype);
    //TODO MAKE internet_archive_remote_put_object RETURN A STATUS AND IF
    //FAILED RETURN FALSE
    //return FALSE;
  }
  //PUTTING UP A LOCAL FILE
  else {
    dsm('pushing local file');
    $status = $s3->putObjectFile($file_name,$config['internet_archive_bucket'],
				 basename($file_name), $headers, $mimetype);
    //IF WE HAD A HARD FAIL ON PUTTING UP THE FILE
    if (!$status) {
      $archive_data['status'] = ARCHIVE_FAILED;
      
      //UPDATE internet_archive TABLE WITH FAILURE INFO
      if (isset($archive['tid'])) {
	internet_archive_update_archive_data($archive_data);
      }
      else {
	$archive_data['tid'] =
	  internet_archive_insert_archive_data($archive_data);
      }
      
      $error =
	t('File transfer to Archive.org failed, transfer marked as failed.');

      watchdog('internet_archive', $error, NULL, WATCHDOG_ERROR);

      return FALSE;
    }
  }

  // SUCCESS! NOW MARK THE $archive_data AS TRANSFERRED AND ADD OR UPDATE
  // THE ROW IN THE internet_archive TABLE 
  $archive_data['status'] = ARCHIVE_TRANSFERRED;
  
  if (isset($archive_data['tid'])) {
    internet_archive_update_archive_data($archive_data);
  }
  else {
    $archive_data['tid'] = internet_archive_insert_archive_data($archive_data);
  }
  
  internet_archive_invoke_internet_archive($archive_data, 'transferred');

  return TRUE;
}

  
/**
 * @param $archive_data filled with all necessary info for an insert
 */
function internet_archive_update_archive_data($archive_data) {
  dsm($archive_data, 'updating archive data');
  //  dsm(debug_backtrace(), 'Load Archive Data Backtrace');
  
  db_update('internet_archive')
    ->fields(array(
		   'item' => $archive_data['item'],
		   'in_path' => $archive_data['in_path'],
		   'file_name' => $archive_data['file_name'],
		   'file_size' => $archive_data['file_size'],
		   'status' => $archive_data['status'],
		   'attempts' => $archive_data['attempts'],
		   'transfer_initiated' =>
		   $archive_data['transfer_initiated'],
		   'md5' => $archive_data['md5'],
		   'date' => $archive_data['date'],
		   'archive_url' => $archive_data['archive_url'],
		   'nid' => $archive_data['nid'],
		   'derivatives' => isset($archive_data['derivatives']) ?
		   $archive_data['derivatives'] : '',
		   ))
    ->condition('tid', $archive_data['tid'])
    ->execute();
}

/**
 * @param $archive_data filled with all necessary info for an insert
 */
function internet_archive_insert_archive_data($archive_data) {
  dsm($archive_data, 'inserting archive data');
  $sql =
    "INSERT into {internet_archive} ".
    "(item, in_path, file_name, file_size, field_name, file_location, " .
    "status, attempts, transfer_initiated, md5, date, archive_url, nid) " .
    "VALUES (:item, :in_path, :file_name, :file_size, :field_name, " .
    "        :file_location, :status, :attempts, :transfer_initiated, " .
    "        :md5, :date, :archive_url, :nid)";
  
  $args = array(':item' => $archive_data['item'],
		':in_path' => $archive_data['in_path'],
		':file_name' => basename($archive_data['file_name']),
		':file_size' => $archive_data['file_size'],
		':field_name' => $archive_data['field_name'],
		':file_location' => $archive_data['file_location'],
		':status' => $archive_data['status'],
		':attempts' => $archive_data['attempts'],
		':transfer_initiated' => $archive_data['transfer_initiated'],
		':md5' => $archive_data['md5'],
		':date' => $archive_data['date'],
		':archive_url' => $archive_data['archive_url'],
		':nid' => $archive_data['nid']);
  dsm($args, 'my args');

  return db_query($sql, $args, array('return' => Database::RETURN_INSERT_ID));
}

/**
 * Updates an item's metadata at archive.org based on the source file_name.
 *
 * @param $file_name
 *   A source file_name of a file transferred to archive.org
 */
function internet_archive_update_item($node, $archive_data) {
  dsm('internet_archive_update_item');

  //grab all the necessary information
  $config = internet_archive_default_config();

  if (variable_get('internet_archive_debug', FALSE)) {
    watchdog('internet_archive', 'Updating item: ' .
	     $archive_data['item'], NULL, WATCHDOG_NOTICE);
  }

  // load the S3 class
  if (! $s3 = internet_archive_new_s3()) {
    return FALSE;
  }
  $s3->setTransferId($archive_data['tid']);

  dsm($archive_data, 'archive data for node');
  //grab metadata if it is available
  $metadata = array();
  $metadata = internet_archive_get_metadata($node, $archive_data['file_name']);

  $headers = internet_archive_process_metadata($metadata);

  //this tells archive.org to destroy original metadata
  $headers['x-amz-ignore-preexisting-bucket'] = 1;

  $config['internet_archive_bucket'] = $archive_data['item'];
  $s3->bucket = $config['internet_archive_bucket'];

  dsm('about to putBucket with updated metadata');
  // Send new metadata
  $s3->putBucket($config['internet_archive_bucket'], $headers);

  $log_entry = array(
    'tid' => $archive_data['tid'],
    'message' => t('Updated metadata at archive.org'),
    'message_data' => $archive_data,
    's3_data' => $headers,
    'type' => ARCHIVE_LOG_NOTICE,
  );

  internet_archive_log($log_entry);
}

/**
 * Deletes all files in an item at archive.org
 *
 * @param $archive_data
 * An array of archive.org information from one of the 
 * internet_archive_load_data functions.
 */
function internet_archive_delete_item($archive_data) {

  //grab all the necessary information
  $config = internet_archive_default_config();

  // load the S3 class
  if (! $s3 = internet_archive_new_s3()) {
    return FALSE;
  }
  $s3->setTransferId($archive_data['tid']);

  $config['internet_archive_bucket'] = $archive_data['item'];
  $s3->bucket = $config['internet_archive_bucket'];

  $s3->deleteObject($config['internet_archive_bucket'],
		    $archive_data['file_name']);
}


/**
 * Deletes all files in an item at archive.org
 *
 * @param $archive_data
 * An array of archive.org information from one of the 
 * internet_archive_load_data functions.
 */
function internet_archive_delete_files($archive_data) {

  //grab all the necessary information
  $config = internet_archive_default_config();

  // load the S3 class
  if (! $s3 = internet_archive_new_s3()) {
    return FALSE;
  }
  $s3->setTransferId($archive_data['tid']);

  $config['internet_archive_bucket'] = $archive_data['item'];
  $s3->bucket = $config['internet_archive_bucket'];

  $s3->deleteFiles($config['internet_archive_bucket'],
		   $archive_data['file_name']);
}

/**
 * Validates transfers to archive.org by checking the contents of the item
 * via S3
 */
function internet_archive_validate_transfers($nid = null) {
  dsm('internet_archive_validate_transfers');
  //how many times should we attempt to transfer failed files
  $attempts = variable_get('internet_archive_queue_attempts', 0) + 1;
  dsm($attempts, 'attempts');

  if ($nid) {
    //only grab 10 at a time to avoid pounding archive.org
    $result = db_query("SELECT * FROM {internet_archive} ".  
		       "WHERE nid = :nid ".
		       "AND (status = :transferred OR ".
		       "(status = :failed AND attempts <= :attempts)) ".
		       "LIMIT 10",
		       array(':nid' => $nid,
			     ':transferred' => ARCHIVE_TRANSFERRED,
			     ':failed' => ARCHIVE_FAILED,
			     ':attempts' => $attempts));
  }
  else {
    //only grab 10 at a time to avoid pounding archive.org
    $result = db_query("SELECT * FROM {internet_archive} ".  
		       "WHERE (status = :transferred OR ".
		       "(status = :failed AND attempts <= :attempts)) ".
		       "LIMIT 10",
		       array(':transferred' => ARCHIVE_TRANSFERRED,
			     ':failed' => ARCHIVE_FAILED,
			     ':attempts' => $attempts));
  }
  while ($archive_data = $result->fetchAssoc()) {
    $valid = internet_archive_validate_transfer($archive_data);
    if (!$valid && (REQUEST_TIME - $archive_data['date']) > (8*60*60)) {
      $archive_data['status'] = ARCHIVE_FAILED;
      $archive_data['error'] = 
	t('Validation failed, source file does not exist at Archive.org ' .
	  'after 8 hours. Marked as failed.');
      
      internet_archive_update_archive_data($archive_data);
      internet_archive_invoke_internet_archive($archive_data, 'failed');
    }
    elseif ($valid) {
      $archive_data['status'] = ARCHIVE_VALIDATED;
      internet_archive_update_archive_data($archive_data);
      internet_archive_invoke_internet_archive($archive_data, 'validated');
    }
  }
}

/**
 * Validates a single transfer by checking if the item contents are empty
 * at archive.org via S3
 *
 * @param $tid
 * An internet_archive transfer ID
 *
 * @return
 * TRUE or FALSE
 */
function internet_archive_validate_transfer($archive_data) {

  if (!$archive_data) {
    return FALSE;
  }

  $derivatives = internet_archive_get_derivatives($archive_data);

  if (!$derivatives) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Creates a URL to a derivative file WITHOUT checking to
 * see if the file actually exists on archive.org
 *
 * Use internet_archive_derivative if you need a more accurate, but
 * much slower solution.
 *
 * @param $nid
 *   A node ID containing archive.org enabled field data
 * @param @ext
 *   The extension of the derivative you want ex. ogv
 *
 * @return full, assumed url to the derivative or FALSE if no transferred
 * files are found on the node
 */
function internet_archive_simple_derivative($nid, $ext) {
  $archive = db_query("SELECT * FROM {internet_archive} WHERE nid = :nid",
		      array(':nid' => $nid))->fetchAssoc();
  if ($archive['status'] == ARCHIVE_TRANSFERRED) {
    $pathinfo = pathinfo($archive['file_name']);
    return 'http://www.archive.org/download/' . $archive['item'] . '/' .
      $pathinfo['file_name'] . '.' . $ext;
  }
  else {
    return FALSE;
  }
}

/**
 * Attempts to check whether or not an item/bucket has been
 * created at archive.org. Basically calls up the page and does a
 * header check.
 *
 * @param $item_name
 *   A string identifier for the item at archive.org
 *
 * @return
 *   TRUE or FALSE
 */
function internet_archive_bucket_exists($item_name) {
  $bucket_url = 'https://www.archive.org/details/' . $item_name;
  dsm($bucket_url, 'looking for bucket with url');
  if (internet_archive_validate_url($bucket_url)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Gets a list of all items/buckets created via S3
 *
 * @return
 *   An object containing all the bucket/item information
 */
function internet_archive_buckets() {
  $s3 = internet_archive_new_s3();
  $s3->setTransferId(0);

  $buckets = $s3->listBuckets(TRUE);

  return $buckets;
}

/**
 * Gets derivatives for a specific item at archive.org
 *
 * @param $item_name
 *   A string containing the item identifier at archive.org
 * @param $extension
 *   Optional file extension like .mpeg to limit derivative results
 *
 * @return
 *   An object containing all of the derivatives for an item.
 */
function internet_archive_get_derivatives($archive_data, $extension = NULL) {
  dsm('internet_archive_get_derivatives');
  $s3 = internet_archive_new_s3();

  $s3->setTransferId($archive_data['tid']);

  //NEED TO ADD HERE..
  $bucket_info = $s3->getBucket($archive_data['item']);
  if ($bucket_info) {
    if ($extension) {
      foreach ($bucket_info as $file_name => $fileinfo) {
        $pathinfo = pathinfo($file_name);
        if ($pathinfo['extension'] == $extension) {
          return $fileinfo;
        }
      }
      return FALSE;
    }
    else {
      return $bucket_info;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_is_archive_format($file_name) {
  //formats derived by archive.org
  //TODO: add more formats for audio, etc.
  $archive_formats = array('mpeg', 'ogv', 'mp4');
  $pathinfo = pathinfo($file_name);
  if (in_array($pathinfo['extension'], $archive_formats)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Checks for new derivative information from uploaded files and stores that 
 * information as serialized data in the internet_archive table. Stops 
 * checking for metadata when a file has completed deriving.
 *
 * @param $limit
 *   An integer which limits how many files to check in a single run at 
 *   archive.org
 */
function internet_archive_store_derivatives($nid, $limit = 25) {
  dsm('internet_archive_store_derivatives');

  if ($nid) {
    // only grab items that have already been successfully uploaded
    $sql =
      "SELECT * FROM {internet_archive} " .
      "WHERE status = :status " .
      "AND nid = :nid ".
      "AND (derivatives IS NULL OR derivatives <> :derivatives) " .
      "ORDER BY date ASC ";
      $args = array(
		    ':nid' => $nid,
		    ':status' => ARCHIVE_VALIDATED,
		    ':derivatives' => 'failed',
		    );
  }
  else {
    // only grab items that have already been successfully uploaded
    $sql =
      "SELECT * FROM {internet_archive} " .
      "WHERE status = :status " .
      "AND (derivatives IS NULL OR derivatives <> :derivatives) " .
      "ORDER BY date ASC ";
    $args = array(':status' => ARCHIVE_VALIDATED,
		  ':derivatives' => 'failed',
		  );
  }

  $results = db_query_range($sql, 0, $limit, $args);
  
  while ($archive_data = $results->fetchAssoc()) {
    //+1 to account for the original file
    $expected_derivatives =
      variable_get('internet_archive_derivatives', 3) + 1;

    //if we are uploading a file in a format archive.org derives it will not
    //re-derive it therefore we need to decrement the expected derivatives by 1
    if (internet_archive_is_archive_format($archive_data['file_name'])) {
      $expected_derivatives = $expected_derivatives - 1;
    }

    unset($derivatives);
    $derivatives = internet_archive_get_derivatives($archive_data);
    dsm($derivatives, 'found derivatives');
    if ($derivatives && (count($derivatives) >= $expected_derivatives)) {
      $archive_data['status'] = ARCHIVE_DERIVED;
      $archive_data['derivatives'] = serialize($derivatives);
      
      internet_archive_update_archive_data($archive_data);
      internet_archive_invoke_internet_archive($archive_data, 'derived');
    }
    else if ($derivatives && (count($derivatives) > 0)) {
      $archive_data['derivatives'] = serialize($derivatives);
      internet_archive_update_archive_data($archive_data);
    }
    elseif ((REQUEST_TIME - $archive_data['date']) > 172800) {
      // if derivatives are not completed after 2 days, check to see if
      //transfer validates
      $valid = internet_archive_validate_transfer($archive_data['tid']);
      if (!$valid) {
        // if it doesn't validate, mark as failed for reprocessing
	$archive_data['status'] = ARCHIVE_FAILED;
	$archive_data['error'] = 
	t('Validation failed, source file does not exist at Archive.org ' .
	  'after 8 hours. Marked as failed.');
	
	internet_archive_update_archive_data($archive_data);
        internet_archive_invoke_internet_archive($archive_data, 'failed');
      }
      else {
	$archive_data['derivatives'] = 'failed';	
        internet_archive_invoke_internet_archive($archive_data,
						 'derivativefailed');
      }
    }
  }
  return;
}

/**
 * Confirmation for manual internet archive transfer
 *
 * @see internet_archive_transfer_confirm_submit()
 *
 * @ingroup forms
 */
function internet_archive_transfer_confirm($form, $drupal_form, $file_name) {
  $file_name = str_replace('!-!', '/', urldecode($file_name));
  watchdog('internet_archive',
	   'Executing internet_archive_transfer_confirm() with file_name: ' .
	   $file_name);

  $output .= '<br />' .
    t('You are about to transfer: %file_name to Archive.org',
      array('%file_name' => $file_name));
  
  $output .= '<br /><strong>' .
    t('Depending on the size of the file, this could take awhile.') .
    '</strong><br /><br />';

  $form['markup'] = array(
    '#type' => 'markup',
    '#value' => $output,
  );
  $form['file_name'] = array(
    '#type' => 'value',
    '#value' => $file_name,
  );
  
  $form['field_name'] = array(
    '#type' => 'value',
    '#value' => filter_input(INPUT_GET, 'field_name'),
  );
  $form['file_location'] = array(
    '#type' => 'value',
    '#value' => filter_input(INPUT_GET, 'file_location'),
  );
  $form['original_referrer'] = array(
    '#type' => 'value',
    '#value' => $_SERVER['HTTP_REFERER'],
  );

  return confirm_form($form, t('Are you sure you want to transfer this file?'),
		      NULL, $description = '', t('Transfer'), t('Cancel'));
}

/**
 * Takes file data from internet archive utility link and starts
 * a manual file transfer to archive.org
 * TODO: find a better solution for clearing the file cache.
 *
 * @ingroup forms
 */
function internet_archive_transfer_confirm_submit($form_id, &$form_state) {
  dsm('internet_archive_transfer_confirm_submit');

  $data = array(
		'file_name' => $form_state['values']['file_name'],
		'field_name' => $form_state['values']['field_name'],
		'file_location' => $form_state['values']['file_location'],
		);
  $status = internet_archive_default_send($data);

  
  if ($status) {
    drupal_set_message('File transferred successfully, please note there ' .
		       'may be a short delay before the file is available ' .
		       'on Archive.org');
  }
  else {
    drupal_set_message('Error Code #5857 The file: ' .
		       $form_state['values']['file_name'] . ' was not sent.',
		       'error');
  }

  //clear the page cache so updated file information appears on node
  //TODO: there has to be a less drastic way to accomplish the goal
  dsm('did not flush cache');
  //drupal_flush_all_caches();
}

/**
 * Sends a file to archive.org using mostly default settings
 * TODO: remove extra watchdog debug
 *
 * @see internet_archive_default_config()
 *
 * @param $file_name
 *   A string containing a file path.
 *
 * @return
 */
function internet_archive_default_send($file_info) {
  //grab all the necessary information
  $config = internet_archive_default_config();
  //send the file
  $status = internet_archive_send($config, $file_info);

  return $status;
}

/**
 * Builds a default archive.org transfer configuration using information
 * provided on the general settings page.
 *
 * @return $config
 *   An array containing basic s3 configuration info.
 */
function internet_archive_default_config() {
  $config = array();
  $config['internet_archive_bucket'] =
    variable_get('internet_archive_bucket',
		 str_replace(' ', '_', variable_get('site_name',
						    t('My Site'))));
  $config['internet_archive_drupal_file_path'] = 0;
  $config['advanced'] = array(
    'internet_archive_server_url' =>
    variable_get('internet_archive_default_server_url',
		 "https://s3.us.archive.org/"),
  );

  return $config;
}

/**
 * Invoke a hook_internet_archive() operation in all modules.
 *
 * @param &$node
 *   An internet_archive data object.
 * @param $op
 *   A string containing the name of the internet_archive operation.
 * @return
 *   The returned value of the invoked hooks.
 */
function internet_archive_invoke_internet_archive(&$archive_data, $op) {
  $return = array();
  foreach (module_implements('internet_archive') as $name) {
    $function = $name . '_internet_archive';
    $result = $function($archive_data, $op);
    if (isset($result) && is_array($result)) {
      $return = array_merge($return, $result);
    }
    elseif (isset($result)) {
      $return[] = $result;
    }
  }
  return $return;
}

/**
 * Implements hook_internet_archive().
 */
function internet_archive_internet_archive(&$archive_data, $op) {
  switch ($op) {
    case 'harvested':
      break;

    case 'queued':
      if ($archive_data['status'] != ARCHIVE_FAILED) {
        $message = t('Queued for transfer');
      }
      else {
        $message = t('Requeued for transfer due to previous failure, attempt '.
		     '%attempt of %retries',
		     array('%attempt' => $archive_data['attempts'] + 1,
			   '%retries' =>
			   variable_get('internet_archive_queue_attempts', 0)
			   + 1));
      }
      $tid = isset($archive_data['tid']) ? $archive_data['tid'] : NULL;
      $log_data = array(
        'tid' => $tid,
        'message' => $message,
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_NOTICE,
      );
      internet_archive_log($log_data);
      break;

    case 'transferring':
      $log_data = array(
        'tid' => $archive_data['tid'],
        'message' => t('Transfer started to Archive.org'),
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_NOTICE,
      );
      internet_archive_log($log_data);
      break;

    case 'transferred':
      $log_data = array(
        'tid' => $archive_data['tid'],
        'message' => t('Transfer completed to Archive.org'),
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_NOTICE,
      );
      internet_archive_log($log_data);
      break;

    case 'validated':
      $log_data = array(
        'tid' => $archive_data['tid'],
        'message' => t('Validated, source file exists at Archive.org'),
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_NOTICE,
      );
      internet_archive_log($log_data);
      break;

    case 'derived':
      $log_data = array(
        'tid' => $archive_data['tid'],
        'message' => t('Derivative processing at Archive.org completed'),
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_NOTICE,
      );
      internet_archive_log($log_data);
      break;

    case 'failed':
      $log_data = array(
        'tid' => $archive_data['tid'],
        'message' => $archive_data['error'],
        'message_data' => $archive_data,
        'type' => ARCHIVE_LOG_ERROR,
      );
      internet_archive_log($log_data);
      break;
  }
}

/********************************************************
 * Metadata Integration
 ********************************************************
 */

/**
 * Builds an array of metadata by calling hook_internet_archive_metadata
 * TODO: Convert this to hook_internet_archive
 * @param $file_name
 *   A string containing a file path.
 *
 * @return $metadata
 *   An array of metadata keyed by header name
 */
function internet_archive_get_metadata(&$node, $file_name) {
  dsm('internet_archive_get_metadata');
  $metadata = array();

  $results = module_invoke_all('internet_archive_metadata', $node, $file_name);
  foreach ($results as $result) {
    $metadata = array_merge($result, $metadata);
  }

  //if mediatype is not set, try to figure it out manually
  //TODO: add a whole bunch more mimetype mappings to this list.
  if (!isset($metadata['mediatype'])) {
    $pathinfo = pathinfo($file_name);
    $filemime = file_get_mimetype($file_name);
    switch ($filemime) {
      case 'image/jpeg':
        $metadata['mediatype'] = 'Image';
        break;
      case 'audio/mpeg':
        $metadata['mediatype'] = 'audio';
        break;
      case 'video/mp2p':
    case 'video/mp4':
      case 'video/quicktime':
      case 'video/x-flv':
        $metadata['mediatype'] = 'movies';
        break;
      default:
        if (variable_get('internet_archive_debug', FALSE)) {
          watchdog('internet_archive',
		   'Warning, no media type found. Defaulting to: ' .
		   variable_get('internet_archive_mediatype', 'movies'),
		   NULL, WATCHDOG_WARNING);
        }

        $metadata['mediatype'] = variable_get('internet_archive_mediatype',
					      'movies');
        break;
    }
  }

  //if no collection was specified in hook_internet_archive_metadata, check
  //for default in settings
  if (!isset($metadata['collection']) &&
      ($default_collection =
       variable_get('internet_archive_collection', "test_collection"))) {
    $metadata['collection'] = explode(',', $default_collection);
  }

  //if no title has been provided, and administrative setting is set item to
  //node title, set it now
  if (variable_get('internet_archive_bucket_title', FALSE) &&
      !$metadata['title'] && $node->title) {
    $metadata['title'] = $node->title;
  }
  elseif (!$metadata['title']) {
    //finally if no title has been provided, use the file_name.
    $metadata['title'] = basename($file_name);
  }

  //if no description has been provided, and administrative setting is set
  ///description to body, set it now
  if (variable_get('internet_archive_bucket_body', FALSE) &&
      !isset($metadata['description']) && isset($node->body)) {
    $metadata['description'] = $node->body;
  }

  //if no creative commons license has been provided, but it is available
  //in node, set it now
  $uri = ($node && isset($node->cc)) ? $node->cc->uri : NULL;
  if (!isset($metadata['licenseurl']) && $uri) {
    $metadata['licenseurl'] = $node->cc->uri;
  }

  return $metadata;
}

/**
 * Builds archive.org compatible S3 header data.
 *
 * @param $metadata
 *   An array of metadata entries.
 *
 * @return $headers
 *   An array of headers.
 */
function internet_archive_process_metadata($metadata) {

  $headers = array();
  foreach ($metadata as $m_key => $info) {
    if (is_array($info)) {
      $counter = 1;
      foreach ($info as $i_key => $value) {
        $headers['x-amz-meta0' . $counter . '-' . $m_key] =
	  internet_archive_clean_text($value);
        $counter++;
      }
    }
    else {
      $headers['x-amz-meta-' . $m_key] = internet_archive_clean_text($info);
    }
  }

  return $headers;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_thumb_from_file_name($field_name, $field_name) {
  $data = internet_archive_load_data($field_name,
				     $field_name);

  if ((!$item = $data['item']) || $data['status'] != 'derived') {
    return FALSE;
  }

  $pathinfo = pathinfo($data['archive_url']);
  $file_name = $pathinfo['file_name'] . '_000001.jpg';

  $link = 'http://www.archive.org/download/';
  $link .= $item . '/' . $item . '.thumbs/';
  $link .= $file_name;
  return $link;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_animated_thumb_from_file_name($field_name,
						       $field_name) {

  $data = internet_archive_load_data($field_name, $field_name);

  if ((!$item = $data['item']) || $data['status'] != 'derived') {
    return FALSE;
  }

  $pathinfo = pathinfo($data['archive_url']);
  $file_name = $pathinfo['file_name'] . '.gif';

  $link = 'http://www.archive.org/download/';
  $link .= $item . '/';
  $link .= $file_name;
  return $link;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_embed($tid, $width = 470, $height = 371) {
  
  $archive_data = internet_archive_load_data_tid($tid);
  $url = internet_archive_derivative_url($archive_data, 'mp4');

  if ($url) {
    $embed = internet_archive_create_embed($url, $width, $height);
  }
  else {
    return FALSE;
  }

  return $embed;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_derivative_url($item, $extension) {
  $derivatives = unserialize($item['derivatives']);
  if ($derivatives) {
    foreach ($derivatives as $key => $info) {
      $pathinfo = pathinfo($key);
      if ($pathinfo['extension'] == $extension) {
        $file_name = $key;
      }
    }
  }

  if ($file_name) {
    $url = 'http://www.archive.org/download/' . $item['item'] . '/' .
      $file_name;
    return $url;
  }
  else {
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_create_embed($url, $width = 470, $height = 371) {
  $embed = '';
  $pathinfo = pathinfo($url);
  $base_url = $pathinfo['dirname'] . '/';
  $file_name = $pathinfo['basename'];

  //$embed = '<object width="'.$width.'" height="'.$height.'"
  //classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000">';
  $embed .= '<param value="true" name="allowfullscreen"/>';
  $embed .= '<param value="always" name="allowscriptaccess"/>';
  $embed .= '<param value="high" name="quality"/>';
  $embed .= '<param value="true" name="cachebusting"/>';
  $embed .= '<param value="#000000" name="bgcolor"/>';
  $embed .= '<param name="movie" ' .
    'value="http://www.archive.org/flow/flowplayer.commercial-3.2.1.swf" />';

  $embed .= "<param value=\"config={'key':'#\$aa4baff94a9bdcafce8','playlist':['format=Thumbnail?.jpg',{'autoPlay':false,'url':'" . $file_name . "'}],'clip':{'autoPlay':true,'baseUrl':'" . $base_url . "','scaling':'fit','provider':'h264streaming'},'canvas':{'backgroundColor':'#000000','backgroundGradient':'none'},'plugins':{'controls':{'playlist':false,'fullscreen':true,'height':26,'backgroundColor':'#000000','autoHide':{'fullscreenOnly':true}},'h264streaming':{'url':'http://www.archive.org/flow/flowplayer.pseudostreaming-3.2.1.swf'}, 'sharing':{'url':'http://www.denveropenmedia.org/sites/all/plugins/flowplayer/flowplayer.sharing-3.2.1.swf'}},'contextMenu':[{},'-','Flowplayer v3.2.1']}\" name=\"flashvars\"/>";

  $embed .= '<embed src="http://www.archive.org/flow/flowplayer.commercial-3.2.1.swf" type="application/x-shockwave-flash" width="' . $width . '" height="' . $height . '" allowfullscreen="true" allowscriptaccess="always" cachebusting="true" bgcolor="#000000" quality="high" flashvars="config={\'key\':\'#$aa4baff94a9bdcafce8\',\'playlist\':[\'format=Thumbnail?.jpg\',{\'autoPlay\':false,\'url\':\'' . $file_name . '\'}],\'clip\':{\'autoPlay\':true,\'baseUrl\':\'' . $base_url . '\',\'scaling\':\'fit\',\'provider\':\'h264streaming\'},\'canvas\':{\'backgroundColor\':\'#000000\',\'backgroundGradient\':\'none\'},\'plugins\':{\'controls\':{\'playlist\':false,\'fullscreen\':true,\'height\':26,\'backgroundColor\':\'#000000\',\'autoHide\':{\'fullscreenOnly\':true}},\'h264streaming\':{\'url\':\'http://www.archive.org/flow/flowplayer.pseudostreaming-3.2.1.swf\'}},\'contextMenu\':[{},\'-\',\'Flowplayer v3.2.1\']}"></embed>';
  //$embed .= '</object>';

  return $embed;
}

/**
 * Fetches all the cck tables associated with the fields defined in the
 * general settings area.
 *
 * @return $tables
 *   An array of table names
 */
function internet_archive_get_field_tables() {
  $fields = internet_archive_fields();

  foreach ($fields as $field_name => $field_info) {
    $extra = field_info_field($field_name);
    list($table, $table_info) =
      each($extra['storage']['details']['sql'][FIELD_LOAD_CURRENT]);

    switch ($field_info['type']) {
    case 'text':
      $value_key = $table_info['value'];
      break;

    case 'file':
      $value_key = $table_info['fid'];
      
      break;
    case 'filefield':
    case 'emvideo':
      $value_key = NULL;
      drupal_set_message('Only the file and text field types are defined ' .
			 'at this time2', 'warning');
      break;
    
    }
    
    $tables[$table] = array(
			    'name' => $table,
			    'value_column' => $value_key,
			    'type' => $field_info['type'],
			    );
  }
  return $tables;
}

/**
 * Function takes in a file name and field name and then queries for a row
 * in the internet_archive table
 *
 * @param $file_name - the file name
 * @param $field_name - the field name the file is stored in 
 */
function internet_archive_load_data($file_name,
				    $field_name) {
  
  $sql =
    "SELECT * FROM {internet_archive} ".
    "WHERE (file_name = :file_name  OR file_name = :base_name)";
  $args = array(':file_name' => $file_name,
		':base_name' => basename($file_name));

  if ($field_name) {
    $sql .= " AND field_name = :field_name ";
    $args[':field_name'] = $field_name;
  }

  //dsm("Archive Data File Name: $file_name");
  //dsm("Archive Data Field Name: $field_name");
  //dsm($args, $sql);
  $archive_data = db_query($sql, $args)->fetchAssoc();

  if ($archive_data) {
    //dsm('found archive data');
    //dsm($archive_data, 'Archive Data from DB');
  }
  else {
    //dsm('didnt find archive data');
  }

  //dsm(debug_backtrace(), 'Load Archive Data Backtrace');
  
  return $archive_data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_load_data_tid($tid) {
  if (is_numeric($tid)) {
    $archive_data = db_query("SELECT * from {internet_archive} ".
		     "WHERE tid = :tid", array(':tid' => $tid))->fetchAssoc();
    return $archive_data;
  }

  return FALSE;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_derivative_extensions() {
  $result =
    db_query("SELECT * FROM {internet_archive} ORDER BY tid DESC LIMIT 50");

  $extensions = array();
  while ($archive_data = $result->fetchAssoc()) {
    $derivatives = unserialize($archive_data['derivatives']);
    foreach ($derivatives as $key => $info) {
      $pathinfo = pathinfo($key);
      if (!$extensions[$pathinfo['extension']]) {
        $extensions[$pathinfo['extension']] = $info['name'];
      }
    }
  }

  return $extensions;
}

