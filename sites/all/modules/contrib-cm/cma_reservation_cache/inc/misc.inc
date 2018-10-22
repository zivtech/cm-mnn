<?php
/**
 * Function will be passed in a set of information for a cache item slot time
 * This will construct a unique identifier from the information for the slot
 * For Resource items, it will be item nid plus the start time of the slot.
 * For Bucket items, it will be content type plus bucket slot id plus start 
 * time of the slot
 */
function cma_reservation_slot_index($content_type_machine_name,
				    $bucket_slot_id,
				    $item_nid,
				    $cache_slot_start_time) {

  if ($bucket_slot_id > 0) {
    $index = $content_type_machine_name . "||" . $bucket_slot_id . "||" .
      $cache_slot_start_time;
  }
  else {
    $index = $item_nid. "||" . $cache_slot_start_time;
  }
  
  return $index;
}
/**
 * Function add the " ON DUPLICATE KEY UPDATE" part of an Insert sql statement
 */
function cma_reservation_cache_duplicate_key_update($insert_fields) {

  $ret = " ON DUPLICATE KEY UPDATE ";

  $is_first_field = TRUE;
  foreach($insert_fields as $key=>$value) {
    if ($is_first_field) {
      $is_first_field = FALSE;
    }
    else {
      $ret .= ", ";
    }
    $ret .= $value ."=VALUES(" . $value . ")";
  }
  return $ret;
}
/**
 * Function will add a set of VALUES to an INSERT statement
 *
 */
function cma_reservation_cache_add_values_to_query($row,
						   $fields,
						   &$is_first_row,
						   &$sql,
						   &$args,
						   &$counter) {

  //add to insert sql and delete sql depending on first row or subsequent row
  if ($is_first_row) {
    $is_first_row = FALSE;
  }
  else {
    $sql .= ", ";
  }
  
  $sql .= " ( " ;
  
  $is_first_field = TRUE;
  
  //loop through all of our fields in the cache table and pluck values out
  //of this $row
  foreach($fields as $key=>$value) {
    if ($is_first_field) {
      $is_first_field = FALSE;
    }
    else {
      $sql .= ", ";
    }
    $arg_name =  ':' . $value . '_' . $counter;
    $sql .= $arg_name;
    $args[$arg_name] = $row[$value];
    }
  $sql .= " ) ";
  $counter ++;
}
/**
 * Function will set the two Datetime elements passed in by reference to
 * the correct start and end of a cache date range
 */
function cma_reservation_cache_set_range_date_objects(&$cache_start_object,
						      &$cache_end_object) {


  $time = cma_reservation_cache_get_max_cache_date();

  //get start and end from the submitted form values
  $cache_start = date('Y-m-d') . " 00:00:00";
  $cache_end = date('Y-m-d', $time) . " 23:59:59";
  $cache_start_object = new DateTime($cache_start);
  $cache_end_object = new DateTime($cache_end);
}
/**
 * Function finds the unique item identifier, whether its a bucket or 
 * resource
 */
function cma_reservation_cache_slot_identifier($slot) {
  $ret = '';
  if ($slot->bucket_slot_id > 0) {
    $ret = $slot->content_type_machine_name . "||" . $slot->bucket_slot_id;
  }
  else {
    $ret = $slot->content_type . "||" . $slot->item_nid;
  }
  return $ret;
}

/**
 * Function will return the offset between the site timezone and the timezone
 * of the timestamp in the cache table which is UTC
 */
function cma_reservation_cache_get_offset(){
  //find our offset from UTC time which is what is in db
  //FIXME CALL FROM A FUNCTION THAT DEFINES A HOOK TO SET DIFFERENTLY IF
  //NECESSARY, OR MAYBE SET OFF THE RESERVATION FIELD ITSELF
  $offset = date_offset_get(new DateTime);
  return $offset;
}

/**
 * Function finds the item title, whether its a bucket or resource item
 */
function cma_reservation_cache_slot_item_title($slot) {
  $ret = '';
  if ($slot->bucket_slot_id > 0) {
    $ret = $slot->content_type . " #" . $slot->bucket_slot_id;
  }
  else {
    $ret = $slot->item_title;
  }
  return $ret;
}
/**
 * Function finds the background color for the slot
 */
function cma_reservation_cache_slot_background_color($slot) {
  $status_code = $slot->reservation_status;

  if (empty($status_code)) {
    $var_name = "cma_reservation_cache_chart_cell_bg_color_default";
  }
  else {
    $var_name = "cma_reservation_cache_chart_cell_bg_color_" . $status_code;
  }

  $color = variable_get($var_name, 'transparent');

  //FIXME ADD HOOK TO ALLOW FOR CUSTOM BACKGROUND COLORS
  return $color;
}

/**
 * FUNCTION WILL RETURN A SUFFIX FOR AN ARRAY OF IDS, USED FOR INDEXES
 * IN THE CHART OPTIONS VARIABLE IN THE VARIABLE TABLE
 */
function cma_reservation_cache_options_suffix($options) {
  sort($options);
  $suffix = '';
  if (!empty($options)) {
    $first = TRUE;  
    foreach($options as $key =>$id) {
      $suffix .= "_"; 
      $suffix .= $id;
    }
  }
  return $suffix;
}
/** 
 * Function will return all of the fields in the cache table
 */
function cma_reservation_cache_db_table_columns() {
  global $cma_reservation_cache_cache_table_fields;

  if (empty($cma_reservation_cache_cache_table_fields)) {
    $fields = cma_reservation_cache_main_cache_fields();
    $cma_reservation_cache_cache_table_fields =
      cma_reservation_cache_get_db_fields($fields);  
  }
  return $cma_reservation_cache_cache_table_fields;
}
/**
 * Logger function
 */
function cma_reservation_cache_log_query($sql, $args) {
  //FIXME ADD VARIABLE SETTING FOR THIS.
  if (variable_get('cma_reservation_cache_logging_enabled', TRUE)) {

  }
}
/**
 * Logger function
 */
function cma_reservation_cache_log_time($log_item) {
  global $prev_microsec_time_log;
  global $total_microsec_time_log;

  //FIXME ADD A FORM ELEMENT TO THE CONFIG PAGE FOR THIS VARIABLE
  if (variable_get('cma_reservation_cache_logging_enabled', TRUE)) {
    
    $current = round(microtime(true) * 1000);
    $total_microsec_time_log = empty($prev_microsec_time_log) ?
      0 : $total_microsec_time_log;

    $prev_microsec_time_log = empty($prev_microsec_time_log) ?
      $current : $prev_microsec_time_log;
    
    $microsecs = $current - $prev_microsec_time_log;
    
    $total_microsec_time_log += $microsecs;
    
    if ($microsecs > 120000) {
      $msg = round($microsecs/60000, 1) . " minutes ";
    }
    else if ($microsecs > 2000) {
      $msg = round($microsecs/1000, 1) . " seconds ";
    }
    else {
      $msg = $microsecs . " microseconds ";
    }

    $msg .= "of total ";
    if ($total_microsec_time_log > 120000) {
      $msg .= round($total_microsec_time_log/60000, 1) . " minutes ";
    }
    else if ($total_microsec_time_log > 2000) {
      $msg .= round($total_microsec_time_log/1000, 1) . " seconds ";
    }
    else {
      $msg .= $total_microsec_time_log . " microseconds ";
    }
    dsm($log_item, $msg);
    $prev_microsec_time_log = $current;
  }
}
/**
 * Function will be passed two cache db rows, if they are different return
 * true
 */
function cma_reservation_cache_row_changed($new_row, $db_result) {
  //array of keys from the new row that we won't compare against the db result
  $skip_keys = array(
		     'cache_time',
		     'cache_slot_start_time_formatted',
		     'cache_slot_end_time_formatted',
		     );
  //loop through all the keys in our new row
  foreach ($new_row as $key=>$value) {
    //if both are empty, or we are skipping this key,  continue
    if ((empty($db_result->{$key}) && empty($value)) ||
	(in_array($key, $skip_keys))) {
      continue;
    }
    //check to see if a real diff, once we fine one difference we return true
    else if ($db_result->{$key} != $value) {
      return TRUE;
    }
  }
  return FALSE;
}
/**
 * Function will return an array of hours in day
 */
function cma_reservation_cache_hours_array() {
  return array(
	        0 => 'Begin',
	        1 => '1am',
	        2 => '2am',
	        3 => '3am',
		4 => '4am',
	        5 => '5am',
	        6 => '6am',
	        7 => '7am',
	        8 => '8am',
	        9 => '9am',
	        10 => '10am',
	        11 => '11am',
	        12 => '12pm',
	        13 => '1pm',
	        14 => '2pm',
	        15 => '3pm',
	        16 => '4pm',
	        17 => '5pm',
	        18 => '6pm',
	        19 => '7pm',
	        20 => '8pm',
	        21 => '9pm',
	        22 => '10pm',
	        23 => '11pm',
	        24 => 'End',
	       );
}


/**
 * Function will return an array of possible filters for the chart
 */
function cma_reservation_cache_filters() {
  return array(
	       CMA_RESERVATION_CACHE_FIRST_DAY,
	       CMA_RESERVATION_CACHE_LAST_DAY,
	       CMA_RESERVATION_CACHE_BEGIN_HOUR,
	       CMA_RESERVATION_CACHE_END_HOUR,
	       CMA_RESERVATION_CACHE_INVENTORY,
	       CMA_RESERVATION_CACHE_BUCKET_CATEGORY,
	       CMA_RESERVATION_CACHE_ITEM_TYPE,
	       CMA_RESERVATION_CACHE_ITEM_NID,
	       CMA_RESERVATION_CACHE_DOW,
	       CMA_RESERVATION_CACHE_SEPARATE_CHARTS,
	       CMA_RESERVATION_CACHE_MODE,
	       );
}


/**
 * Function array of fields to add to the cma_reservation_cache table. Many of
 * the fields are added by looking at the fields currently on the main 
 * reservations content type
 */
function cma_reservation_cache_get_db_fields($main_cache_fields = array()) {
  $fields = $main_cache_fields;

  $skip_types = array(
		      'viewfield',
		      );

  $skip_names = array(
		      'field_reservations_inventory'
		      );
  $fields_info = field_info_instances('node', 'reservations_reservation');
  foreach ($fields_info as $field_name => $value) {
    $field_info = field_info_field($field_name);
    if (in_array($field_info['type'], $skip_types)) {
      continue;
    }
    if (in_array($field_name, $skip_names)) {
      continue;
    }

    if ($field_info['type'] == 'datetime' &&
	isset($field_info['columns']['value']) &&
	isset($field_info['columns']['value2'])) {

      $fields[$field_name."_start"] =
	cma_reservation_cache_db_field_type_info($field_info);
      $fields[$field_name."_end"] =
	cma_reservation_cache_db_field_type_info($field_info);
      
    }
    else {
    $fields[$field_name] =
      cma_reservation_cache_db_field_type_info($field_info);

    }
    unset($fields[$field_name]['not null']);
  }
  return $fields;
}

/**
 * Returns the main cache fields for the cma_reservation_cache table. The other
 * fields on the reservations_reservation will be used for the other columns
 */
function cma_reservation_cache_main_cache_fields() {
  $ret = array();
   
  $ret['cache_id'] = array('type' => 'serial',
			   'description' => 'Cache Primary Key',
			      );
  $ret['cache_time'] = array(
			     'type' => 'datetime',
			     'mysql_type' => 'datetime',
			     'not null' => TRUE,
			     'description' =>
			     'The time the cache was written',
			     );
  $ret['content_type'] =
    array(
	  'type' => 'varchar',
	  'length' => 255,
	  'default' => '',
	  'not null' => TRUE,
	  'description' =>
	  'human readable version of the reservable content type of the item',
	  );
  $ret['content_type_machine_name'] =
    array(
	  'type' => 'varchar',
	  'length' => 255,
	  'default' => '',
	  'not null' => TRUE,
	  'description' =>
	  'machine name of the reservable content type of the item',
	  );

  $ret['bucket_slot_id'] = array(
				 'type' => 'int',
				 'unsigned' => TRUE,
				 'not null' => TRUE,
				 );
  
  $ret['cache_slot_start_time'] = array(
			     'type' => 'int',
			     'unsigned' => TRUE,
			     'not null' => TRUE,
			     );
  $ret['cache_slot_end_time'] = array(
			   'type' => 'int',
			   'unsigned' => TRUE,
			   'not null' => TRUE,
			   );
  $ret['cache_slot_status'] = array(
				    'type' => 'int',
				    'unsigned' => TRUE,
				    'not null' => TRUE,
				    );
  $ret['inventory'] = array(
			    'type' => 'int',
			    'unsigned' => TRUE,
			    );
  $ret['inventory_name'] = array(
				 'type' => 'varchar',
				 'length' => 255,
				 'default' => '',
				 'description' =>
				 'Term name of inventory',
			     );

  $ret['bucket_category'] = array(
				  'type' => 'int',
				  'unsigned' => TRUE,
				  );
  $ret['bucket_category_name'] = array(
				       'type' => 'varchar',
				       'length' => 255,
				       'default' => '',
				       'description' =>
				       'Term name of bucket category',
			     );
    
  $ret['bucket_weight'] = array(
				'type' => 'int',
				'unsigned' => FALSE,
				);
  $ret['item_nid'] = array(
			   'type' => 'int',
			   'unsigned' => TRUE,
			   );
  $ret['item_title'] = array(
			     'type' => 'varchar',
			     'length' => 255,
			     'default' => '',
			     'description' =>
			     'Title of the node of a reservable content '.
			     'type',
			     );
  $ret['item_status'] = array(
			      'type' => 'int',
			      'unsigned' => TRUE,
			      );

  $ret['reservation_nid'] = array(
				  'type' => 'int',
				  'unsigned' => TRUE,
				  );
  $ret['reservation_group_id'] = array(
				       'type' => 'int',
				       'unsigned' => TRUE,
				       );
  $ret['reservation_uid'] = array(
				  'type' => 'int',
				  'unsigned' => TRUE,
				  );
  $ret['reservation_user_name'] = array(
					'type' => 'varchar',
					'length' => 255,
					'default' => '',
					'description' =>
					'User name of the person whose name  '.
					'reservation the reservation is in',
				);

  $ret['reservation_status'] = array(
				     'type' => 'int',
				     'unsigned' => TRUE,
				     );
  
  $ret['reservation_status_name'] = array(
					  'type' => 'varchar',
					  'length' => 255,
					  'default' => '',
					  'description' =>
					  'Name of the status of the '.
					  'reservation',
					  );
  $ret['project_nid'] = array(
			      'type' => 'int',
			      'unsigned' => TRUE,
			      );
  $ret['project_title'] = array(
				'type' => 'varchar',
				'length' => 255,
				'default' => '',
				'description' =>
				'Title of the project node associated  '.
				'with the reservation',
				);

  return $ret;
}

/**
 * Function is passed an array of information about the drupal field,
 * it uses that information to create and array for use in hook_schema
 *
 * @param $field_info drupal field info array
 * @return $info db field info array
 */
function cma_reservation_cache_db_field_type_info($field_info) {
  $info = array();
  switch ($field_info['type']) {
  case 'computed':
    $db_settings = $field_info['settings']['database'];
    $info['type'] = $db_settings['data_type'];
    //find computed field settings to set our db cache field settings
    if ($db_settings['data_type'] == 'int' ||
	$db_settings['data_type'] == 'float' ) {
      $info['size'] = $db_settings['data_size'];
    }
    if ($db_settings['data_type'] == 'varchar' ||
	$db_settings['data_type'] == 'text' ) {
      if ($field_info['settings']['store'] == 1) {
	$info['length'] = $db_settings['data_length'];	
      }
      else {
	//FIXME WHAT SHOULD I SET IT TO? HARD TO SAY SINCE THIS COMPUTED FIELD
	//IS NOT ACTUALLY STORED IN DB
	$info['length'] = 1000;
      }
    }
    else if ($db_settings['data_type'] == 'numeric') {
      $info['precision'] = $db_settings['data_precision'];
      $info['scale'] = $db_settings['data_scale'];
    }
    
    break;
    
  case 'entityreference':
    $info = $field_info['columns']['target_id'];
    break;
    
  case 'taxonomy_term_reference':
    $info = $field_info['columns']['tid'];
    break;

  case 'datetime':
  case 'number_integer':
  case 'text':
  case 'text_long':
  default:
    $info = $field_info['columns']['value'];
    break;
    
  }
  return $info;
}

/**                                                                            
 * Helper function to get a single value off of a entity  
*/
function cma_reservations_cache_get_single_field_value($entity, $field_name,
						       $index = 'value') {
  if (!empty($entity->{$field_name})) {
    $field = $entity->{$field_name};
    if (!empty($field[LANGUAGE_NONE]) &&
        !empty($field[LANGUAGE_NONE][0]) &&
        !empty($field[LANGUAGE_NONE][0][$index])) {
      return $field[LANGUAGE_NONE][0][$index];
    }
  }
  return NULL;
}
