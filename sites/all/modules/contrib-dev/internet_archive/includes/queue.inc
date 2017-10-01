<?php


/**
 * Loads the queue page.
 *
 */
function internet_archive_queues_home() {
  return drupal_get_form('internet_archive_queue_admin_form');
}

/**
 * @file
 * Drupal Queue integration for Internet Archive Module.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_queue_settings_form($form, &$form_state) {
  $description =
    t('The settings below provide basic automation of file transfers to '.
      'Archive.org. These settings are in addition to any configurations you '.
      'or crontab settings for your Drupal Queue. In '.
      'other words, if you have configured your drupal queue crontab to '.
      'run every 15 minutes, but selected only Sundays below -- files will '.
      'be transferred every 15 minutes on Sundays only.') . '</div>';
  
  $form['internet_archive_queue'] = array(
    '#type' => 'fieldset',
    '#title' => 'Archive.org Queue Management Settings',
    '#description' => $description,
  );
  if (module_exists('views')) {
    $description =
      t('This setting allows you to use views to automatically select '.
	'files to add to the transfer queue. Each view selected should '.
	'contain one of the fields specified on the '.
	'<a href="/admin/settings/internet_archive">Main tab</a>. '.
	'Node views should have row style set to fields, but most styles '.
	'(unformatted, table, etc.) should work. Instead of Node, you can '.
	'also use a File view type, just make sure to include the fid field '.
	'in the display. In a standard configuration, files will be  '.
	'harvested when cron is run.');
    
    $form['internet_archive_queue']['internet_archive_file_views'] =
      array(
	    '#type' => 'select',
	    '#title' =>
	    t('Automatically add files to the queue from the following '.
	      'file views'),
	    '#default_value' =>
	    variable_get('internet_archive_file_views', FALSE),
	    '#options' => internet_archive_file_views(),
	    '#multiple' => TRUE,
	    '#description' => $description,
	    );
    $description =
      t('Checking this will cause the queue to be populated via the above '.
	'views on Drupal cron runs. If this is not checked, you will need to '.
	'either harvest files manually or configure Drush and use the '.
	'ia-transfer command which includes the harvest from views step.');
    
    $form['internet_archive_queue']['internet_archive_queue_harvest_cron'] =
      array(
	    '#type' => 'checkbox',
	    '#title' => t('Add files to queue on Drupal cron runs?'),
	    '#description' => $description,
	    '#default_value' =>
	    variable_get('internet_archive_queue_harvest_cron', FALSE),
    );
    $description =
      t('You only need to set this if the views you are harvesting from '.
	'above have data that is not accessible to anonymous users.');
    $form['internet_archive_queue']['internet_archive_cron_user'] =
      array(
	    '#type' => 'textfield',
	    '#title' =>t('Which user should queue processing be executed as?'),
	    '#size' => 30,
	    '#maxlength' => 60,
	    '#autocomplete_path' => 'user/autocomplete',
	    '#default_value' => variable_get('internet_archive_cron_user',
					     FALSE),
	    '#description' => $description,
    );
  }
  $description =
    t('Transfers are relatively reliable to archive.org, but in some '.
	'cases when traffic is heavy a file transfer may not complete. If '.
	'this setting is not zero, the queue will attempt to retransfer  '.
	'ailed files.');
  
  $form['internet_archive_queue']['internet_archive_queue_attempts'] =
    array(
	  '#type' => 'textfield',
	  '#title' =>
	  t('If a file transfer fails, how many additional attempts should '.
	    'be made'),
	  '#default_value' => variable_get('internet_archive_queue_attempts',
					   0),
	  '#description' => $description,
	  '#size' => 5,
  );
  $description =
    t('This is not generally necessary but may be helpful if you are '.
      'transferring very large files and are seeing some files never '.
      'start / stay on queued status for long periods of time.');
  
  $form['internet_archive_queue']['internet_archive_expired_queue'] =
    array(
	  '#type' => 'checkbox',
	  '#title' =>
	  t('Requeue items that expire in the queue before transfer?'),
	  '#description' => $description,
	  '#default_value' => variable_get('internet_archive_expired_queue',
					   FALSE),
  );
  $description =
    t('If no days are selected, transfers will any day that the queue '.
	'cron is executed');
  
  $form['internet_archive_queue']['internet_archive_queue_days'] =
    array(
	  '#type' => 'select',
	  '#title' => 'Select the day/s on which files should be transferred',
	  '#default_value' => variable_get('internet_archive_queue_days',
					   'none'),
	  '#description' => $description,
	  '#options' => internet_archive_day_options(),
	  '#multiple' => TRUE,
  );
  $form['internet_archive_queue']['internet_archive_queue_start_time'] =
    array(
	  '#type' => 'select',
	  '#title' => 'Select the hour that transfers should start processing',
	  '#default_value' => variable_get('internet_archive_queue_start_time',
					   'none'),
	  '#options' => internet_archive_hour_options(),
	  '#multiple' => FALSE,
  );

  $form['internet_archive_queue']['internet_archive_queue_end_time'] =
    array(
	  '#type' => 'select',
	  '#title' =>
	  t('Select the hour that the transfers should stop processing'),
	  '#default_value' => variable_get('internet_archive_queue_end_time',
					   'none'),
	  '#options' => internet_archive_hour_options(),
	  '#multiple' => FALSE,
	  );
  $description =
    t('This setting can be important if you have a slower internet '.
      'connection, or are sending very large files to archive.org. '.
      'Setting this to a small number can help prevent timeouts.');
  
  $form['internet_archive_queue']['internet_archive_queue_limit'] =
    array(
	  '#type' => 'textfield',
	  '#title' =>
	  t('Enter the maximum number of items to send to archive.org in a '.
	    'single cron run'),
	  '#default_value' => variable_get('internet_archive_queue_limit','1'),
	  '#description' => $description,
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
function internet_archive_queue_admin_form($form, &$form_state) {

  $queue_name = 'internet_archive_send_file';
  $items = internet_archive_retrieve_queue($queue_name);

  $form['internet_archive_queue_monitor'] = array(
    '#type' => 'fieldset',
    '#title' => 'Archive.org Queue Monitor' .
    internet_archive_help_links('upload-queue-monitor'),
    '#description' =>
    internet_archive_help_links('upload-queue-monitor', 'link'),
  );

  $form['queue_name'] = array(
    '#type' => 'hidden',
    '#value' => $queue_name,
  );

  $form['harvest_files'] = array(
    '#type' => 'submit',
    '#value' => t('Harvest files available for transfer'),
    '#submit' => array('internet_archive_harvest_files_for_queue'),
  );
  $form['transfer_files'] = array(
    '#type' => 'submit',
    '#value' => t('Transfer items in queue'),
    '#submit' => array('internet_archive_transfer_queue_items'),
  );
  $form['delete_queue'] = array(
    '#type' => 'submit',
    '#value' => t('Empty the Queue'),
    '#submit' => array('internet_archive_clear_queue'),
  );
  $form['status_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Queue status'),
    '#description' =>
    t('Displays items in the queue to be transfered to archive.org'),
    '#collapsible' => TRUE,
  );

  $rows = array();
  foreach($items as $item) {
    $data = unserialize($item['data']);
    $file_name = $data['file_name'];
    $field_name = $data['field_name'];
    $file_location = $data['file_location'];

    $node = internet_archive_get_node_from_file_name($file_name,
						     $field_name);
    $node_link = $node ? l($node->title, "node/".$node->nid) : 'NA';

    $archive_data = internet_archive_load_data($file_name, $field_name);
    
    $expire = $item['expire'] ?
      t('Claimed: expiress %expire',
	array('%expire' => date('m/d/Y h:ia', $item['expire']))) :
      t('Unclaimed');

    $link = isset($archive_data['tid']) ?
		  l(t("Transfer from Queue to Archive.org"),
		    "/ia/transfer/" . $archive_data['tid']) : '';
    
    $created = $item['created'] ? date('m/d/Y h:ia', $item['created']) : '';
    $rows[] = array($node_link, $file_name, $field_name, $file_location,
		    $expire, $created, $link); 
  }
  
  $header = array(
		  t('Node'),
		  t('File Name'),
		  t('Field Name'),
		  t('File Location'),
		  t('Claimed/Expiration'),
		  t('Created Date'),
		  t('Action'),
		  );
  
  $table = theme('table',array('header'=> $header, 'rows' => $rows));
  $form['status_fieldset']['status'] =
    array(
	  '#type' => 'markup',
	  '#markup' => $table,
	  );

  return $form;
}

/**
 * Submit function for "Harvest files available for transfer" button.
 */
function internet_archive_harvest_files_for_queue($form, &$form_state) {
  internet_archive_harvest_files();

  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $count = $queue->numberOfItems();
  drupal_set_message(t('Attempted to harvest files. There are now '.
		       '@count items in the queue.',
		       array('@count' => $count)));

}

/**
 * Submit handler for clearing/deleting the queue.
 */
function internet_archive_clear_queue($form, &$form_state) {
  $queue = DrupalQueue::get($form_state['values']['queue_name']);
  $queue->deleteQueue();

  db_delete('internet_archive')
  ->condition('status', 'queued')
  ->execute();
  drupal_set_message(t('Deleted the @queue_name queue and all items in it.',
		       array('@queue_name' =>
			     $form_state['values']['queue_name'])));
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
function internet_archive_retrieve_queue($queue_name) {
  $items = array();

  $result =
    db_query("SELECT item_id, data, expire, created ".
	     "FROM {queue} WHERE name = :name ORDER BY item_id",
	     array(':name' => $queue_name));

  while ($item = $result->fetchAssoc()) {
    $items[] = $item;
  }
  return $items;
}

/**
 * Function will rely on queue settings to decide if and how many queue
 * items to process.
 *
 */
function internet_archive_transfer_queue_items() {
  global $internet_archive_queue_count;

  $internet_archive_queue_count = $internet_archive_queue_count ?
    $internet_archive_queue_count : 0;

  $limit = variable_get('internet_archive_queue_limit', '1');

  if (!internet_archive_hour_check() || !internet_archive_day_check()) {
    //setting the time range to negative causes the queue to skip execution
    $time = -60;
  }
  else {
    $time = 10800;
  }
  $lease = 9000; //Seconds allowed per transfer task
  $queue_name = 'internet_archive_send_file';
  $end = REQUEST_TIME + $time;
  $items = array();
  $queue = DrupalQueue::get($queue_name);
  while (REQUEST_TIME < $end && ($internet_archive_queue_count <= $limit)) {
    $internet_archive_queue_count ++;
    $item = $queue->claimItem($lease);
    if (!$item) {
      continue;
    }
    $file_name = $item->data['file_name'];
    $field_name = $item->data['field_name'];

    $archive_data = internet_archive_load_data($file_name, $field_name);
    $status = $archive_data['status'];
    
    if ($status != ARCHIVE_TRANSFERRING) {
      $transfer_status = internet_archive_transfer_file_worker($item->data);
      if (!$transfer_status) {
	$queue->deleteItem($item);
	$msg =t("!filename could not be transferred and marked as 'failed'.",
		array('!filename' => $archive_data['file_name']));
	drupal_set_message($msg, 'error');
	continue;
      }
      $queue->deleteItem($item);
    }
    $items[] = $item;
  }

  if (count($items) > $limit) {
    $msg = t('Completed transfering @count files and reached limit of @limit.',
	     array('@count' => $internet_archive_queue_count - 1,
		   '@limit' => $limit)) ;
    drupal_set_message($msg, 'warning');
    return;
  }

  drupal_set_message(t('Completed transfering @count files.',
		       array('@count' => count($items))), 'warning');
  return;

}

/**
 * Primary file harvest function, grabs candidates from views based on 
 * administrative settings and adds them to a drupal queue for later transfer.
 */
function internet_archive_harvest_files() {
  //Check to see if we should authenticate
  if ($cron_user = variable_get('internet_archive_cron_user', FALSE)) {
    global $user;
    $original_user = $user;
    drupal_save_session(FALSE);
    $tmp = array();
    $users = array('name' => $cron_user);
    $array = user_load_multiple($tmp, $users);
    $user = array_shift($array);
  }

  //administrative setting for requeing stale/expired queue items
  if (variable_get('internet_archive_expired_queue', FALSE)) {
    internet_archive_harvest_expired_queued();
  }

  //THIS HOOK WILL ALLOW OTHER MODULES, ESPECIALLY internet_archive_remote,
  //TO FETCH FILES FROM A REMOTE SERVER AND STORE ON A NODE SO THEY WILL
  //BE CAPTURED WHEN WE HARVEST TO THE QUEUE BELOW
  module_invoke_all('internet_archive_harvest_remote_files');
  
  //FIND FILE NAMES FROM ENTITY/NODE FIELDS THAT DO NOT YET EXIST IN THE
  //INTERNET ARCHIVE TABLE
  $file_names = internet_archive_harvest_file_names_from_views();

  //FIND ROWS IN THE INTERNET ARCHIVE TABLE THAT ARE IN A FAILED STATE BUT
  //ARE STILL ALLOWED MORE ATTEMPTS TO ENTER THE QUEUE
  $allowed_attempts = variable_get('internet_archive_queue_attempts', 0);
  if ($allowed_attempts > 0) {
    $reprocess_file_names =
      internet_archive_harvest_failed_for_queue($allowed_attempts);

    if ($reprocess_file_names) {
      $file_names = array_merge($reprocess_file_names, $file_names);
    }
  }
  
  $transfer_queue = DrupalQueue::get('internet_archive_send_file');

  if ($file_names) {
    foreach ($file_names as $file_name => $file_info) {
      $field_name = $file_info['field_name'];
      $file_location = $file_info['file_location'];

      //grab any existing information available for this file_name
      $archive_data = internet_archive_load_data($file_name, $field_name);

      if ($archive_data) {
	$status = $archive_data['status'];
	if (isset($archive_data['nid'])) {
	  $node = node_load($archive_data['nid']);
	}
      }
      else {
	$node = internet_archive_get_node_from_file_name($file_name,
							 $field_name);
	$status = '';
	$file_size = isset($file_size) && ($file_size > 0) ? $file_size : 0;
	$archive_data =
	  array(
		'in_path' => $file_name,
		'file_name' => basename($file_name),
		'file_size' => $file_size,
		'field_name' => $field_name,
		'file_location' => $file_location,
		'md5' => internet_archive_get_md5_for_file($file_name,
							   $file_location),
		'transfer_initiated' => 0,
		'attempts' => 0,
		'date' => REQUEST_TIME,
		'item' => '',
		'nid' => $node->nid,
		'archive_url' => '',
		);
	
      }

      if ($status != ARCHIVE_QUEUED && $status != ARCHIVE_TRANSFERRING
	  && $status != ARCHIVE_TRANSFERRED &&
	  $status != ARCHIVE_VALIDATED && $status != ARCHIVE_DERIVED) {
	if ($file_info['file_location'] == ARCHIVE_FILE_LOCATION_LOCAL &&
	    is_readable($file_name)) {
	  $archive_data['file_size'] = internet_archive_file_size($file_name);
	}
	else {
	  $archive_data['file_size'] = 0;
	}
	
	internet_archive_invoke_internet_archive($archive_data, 'harvested');

	//PREPARE QUEUE ITEM
	$queue_item = array();
	$queue_item = array('file_name' => $file_name,
			    'field_name' => $archive_data['field_name'],
			    'file_location' => $archive_data['file_location']);

	//NEED TO MAKE SURE ITEM IS NOT ALREADY IN THE QUEUE
	$sql = "SELECT item_id FROM {queue} WHERE data = :data";
	$params = array(':data' => serialize($queue_item));
	$queue_id = db_query($sql, $params)->fetchField();
	
	if ($queue_id) {
	  continue;
	}
	//add the file to queue
	$transfer_queue->createItem($queue_item);

	$archive_data['status'] = ARCHIVE_QUEUED;
	if (!isset($archive_data['tid'])) {
	  $archive_data['tid'] =
	    internet_archive_insert_archive_data($archive_data);
	}
	else {
	  internet_archive_update_archive_data($archive_data);
	}
	internet_archive_invoke_internet_archive($archive_data, 'queued');
      }
    }
  }

  //All done, reset our user session if necessary
  if ($cron_user) {
    $user = $original_user;
  }
}


/**
 * Harvests failed files and adds them back to the transfer queue
 * based on number of allowed attemps in administrative settings
 */
function internet_archive_harvest_failed_for_queue($allowed_attempts) {
  $allowed_attempts++;

  $result =
    db_query("SELECT * FROM {internet_archive} " .
	     "WHERE status = :status AND attempts < :attempts",
	     array(':status' => ARCHIVE_FAILED,
		   ':attempts' => $allowed_attempts));

  $file_names = array();
  while ($archive_data = $result->fetchAssoc()) {
    $file_names[$archive_data['in_path']] =
      array(
	    'field_name' => $archive_data['field_name'],
	    'file_location' => $archive_data['file_location'],
	    );
  }

  return $file_names;
}

/**
 * Optional administrative setting looks for items in the
 * internet_archive table that are set as queued, never started transferring
 * and no longer exist in the current drupal queue. For sites with very large
 * files this may be necessary in cases that the drupal queue expires before
 * the items are sent.
 *
 * @return $candidates
 *   An array of file_names to be queued
 */
function internet_archive_harvest_expired_queued() {
  $candidates = array();

  $queue_items = internet_archive_retrieve_queue('internet_archive_send_file');
  foreach ($queue_items as $queue_item) {
    $data = unserialize($queue_item['data']);
    $queue[$data['file_name']] = $data;
  }

  //grab all the queued non-transferred items from internet_archive table
  $ia_result =
    db_query("SELECT * FROM {internet_archive} " .
	     "WHERE status = :status " .
	     "AND transfer_initiated = :transfer_initiated",
	     array(':status' => 'queued', ':transfer_initiated' => 0));
  
  while ($archive_data = $ia_result->fetchAssoc()) {
    if (!isset($queue[$data['file_name']])) {
      $candidates[$archive_data['in_path']] = $archive_data;
    }
  }

  //take candidates and re-add them to the queue
  if ($candidates) {
    $transfer_queue = DrupalQueue::get('internet_archive_send_file');

    foreach ($candidates as $file_name => $archive_data) {
      $field_name = $archive_data['field_name'];
      $file_location = $archive_data['field_location'];
      if ($file_location == ARCHIVE_FILE_LOCATION_LOCAL &&
	  !file_exists($file_name)) {
	continue;
      }
      
      $message = t('Added transfer back to queue due to stale queue status.');
      $log_data = array(
			'tid' => $archive_data['tid'],
			'message' => $message,
			'message_data' => $archive_data,
			'type' => ARCHIVE_LOG_NOTICE,
			);
      
      //add the file to queue
      $queue_item = array(
			  'file_name' => $file_name,
			  'field_name' => $archive_data['field_name'],
			  'file_location' => $archive_data['file_location'],
			  );
      $transfer_queue->createItem($queue_item);
      internet_archive_log($log_data);
    }
  }
  else {
    return FALSE;
  }
}


/**
 * Builds an array of file_names from views chosen on the general 
 * settings page.
 *
 * @return $unarchived_files
 *   An associated array of field info and their value file_names.
 */
function internet_archive_harvest_file_names_from_views() {
  $views = variable_get('internet_archive_file_views', NULL);

  if (!$views) {
    return FALSE;
  }

  //GET ALL THE FIELDS THAT THAT WILL PUSH DATA TO INTERNET ARCHIVE
  $archive_fields = internet_archive_fields();

  //GLOBAL SET IN internet_archive_fields()
  global $archive_fields_info;
  
  //right now only text and file fields are supported
  $field_keys_by_type = array(
			   'text' => array(),
			   'file'=>array()
			   );

  //LOOP OVER EACH VIEW THAT CAN HARVEST ENTITY FIELDS THAT ARE CONFIGURED TO
  //PUSH DATA TO Archive.org
  foreach ($views as $view_name => $info) {
    $harvest_results = views_get_view_result($view_name);

    if (count($harvest_results) > 0) {
      //FIND THE FIELDS THAT HAVE OUR FILE INFO
      foreach ($archive_fields as $field_name => $field_info) {
	
	foreach ($harvest_results[0] as $key => $info) {
	  if ("field_" . $field_name == $key) {
	    $field_keys_by_type[$field_info['type']][$field_name] = $key;
	    break;
	  }
	}
      }    

      //IF WE DONT FIND A FIELD KEY TO GRAB FILE INFO THEN THIS VIEW IS
      //NOT CONFIGURED CORRECTLY
      if (!$field_keys_by_type) {
	drupal_set_message("Error Code #33453 No field found for $view_name",
			   'error');
        continue;
      }

      //LOOP OVER ALL THE RESULTS WE GOT FROM VIEWS
      foreach ($harvest_results as $key => $fileinfo) {
	//LOOP OVER ALL THE RELEVANT FIELD KEYS FOR WITHIN A VIEW RESULT
	foreach($field_keys_by_type as $field_type => $field_keys) {
	  //LOOP THROUGH ALL THE FIELD KEYS & GRAB THE VALUES FROM VIEW RESULT
	  foreach($field_keys as $field_name => $field_key) {
	    $field_value = $fileinfo->{$field_key};

	    //WE HAVE A VALUE FOR OUR FIELD
	    if ($field_value) {
	      $file_location =
		isset($archive_fields_info[ARCHIVE_FILE_LOCATION_REMOTE]
		      [$field_name]) ? ARCHIVE_FILE_LOCATION_REMOTE :
		ARCHIVE_FILE_LOCATION_LOCAL;
	      
	      //IF WE HAVE A 'file' FIELD
	      if ($field_type == 'file') {
		$uri = $field_value[0]['raw']['uri'];
		if (file_exists($uri) ||
		    ($file_location == ARCHIVE_FILE_LOCATION_REMOTE)) {
		  $file_names[$uri] = array('field_name' => $field_name,
					   'file_location' => $file_location);
		}
	      }
	      //IF WE HAVE A VALUE FOR A 'text' FIELD
	      else if($field_type == 'text') {
		$uri = $field_value[0]['raw']['value'];
		$file_names[$uri] = array('field_name' =>$field_name,
					 'file_location' => $file_location);
	      }
	      //WE HAVE A VALUE BUT NO LOGIC SET UP FOR THE $field_type
	      else {
		drupal_set_message("Error Code #8268: Can't find file_name " .
				   "for field of type $field_type", 'error');
	      }
	    }
	  }
	}
      }
    }
  }

  $unarchived_files = array();

  //IF WE FOUND VALUES FOR Archive.org FIELDS, CHECK TO SEE IF THEY THEY
  //ALREADY EXIST IN THE internet_archive CUSTOM TABLE
  if ($file_names) {
    foreach ($file_names as $file_name => $file_info) {
      $archive_data = internet_archive_load_data($file_name,
						 $file_info['field_name']);
      if ((!$archive_data)) {
        $unarchived_files[$file_name] = $file_info;
      }
    }
  }

  return $unarchived_files;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_hour_options() {
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
function internet_archive_day_options() {
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
 * This is a drush function
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_transfer_queue() {

  //validate previous transfers
  internet_archive_validate_transfers();

  //Add new files to queue if available
  internet_archive_harvest_files();

  //transfer files available in the queue
  internet_archive_transfer_queue_items();

  //Check for new derivations to update.
  internet_archive_store_derivatives();
}

/**
 * This function is called by the /ia/transfer_item menu hook and will
 * allow a item to be taken out of the queue and transferred
 */
function internet_archive_transfer_item_out_of_queue($tid) {
  if (!$tid) {
    return;
  }
  $archive_data = internet_archive_load_data_tid($tid);

  if (!$archive_data) {
    return;
  }
  $queue_item = array('file_name' => $archive_data['in_path'],
		      'field_name' => $archive_data['field_name'],
		      'file_location' => $archive_data['file_location'],
		      );

  $transfer_status = internet_archive_transfer_file_worker($queue_item);
  if (!$transfer_status) {
    $msg =t("!filename could not be transferred and marked as 'failed'.",
	    array('!filename' => $archive_data['file_name']));
    drupal_set_message($msg, 'error');
  }
  $sql = "DELETE FROM queue WHERE data=:data";
  $args = array(':data' => serialize($queue_item));
  db_query($sql, $args);
  drupal_goto('node/'. $archive_data['nid']);
}


