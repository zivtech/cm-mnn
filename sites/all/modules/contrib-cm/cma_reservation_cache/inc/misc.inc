<?php
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
 * Function will return the increcment value from the date field on the
 * reservation content type
 */
function cma_reservation_cache_date_field_increment() {
  $reservation_fields =
    field_info_instances('node', 'reservations_reservation');

  $increment = $reservation_fields['field_reservations_date']
    ['widget']['settings']['increment'];

  return $increment;
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
 * Function will return array of bucket categories sorted with their weight 
 * as array index
 */
function cma_reservation_cache_get_bucket_category_weights() {

  $vid = variable_get('reservations_grouping_vid', 0);
  $terms = taxonomy_get_tree($vid);
  $bucket_categories = array();
  //set to arbitrarly large number for terms that haven't yet been weighted
  //on the config page admin/config/cma_reservation_cache/default
  $default_index = 1000000;
  foreach ($terms as $term) {
    $weight = variable_get($term->tid.'_rank_weight', $default_index++);
    $bucket_categories[$term->tid] = $weight;
  }
  return $bucket_categories;
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

function cma_reservation_cache_log_microsec($label = 'Microseconds expired:') {
  global $prev_microsec_time_log;
  global $total_microsec_time_log;
  
  $current = round(microtime(true) * 1000);
  $total_microsec_time_log = empty($prev_microsec_time_log) ?
    0 : $total_microsec_time_log;

  $prev_microsec_time_log = empty($prev_microsec_time_log) ?
    $current : $prev_microsec_time_log;

  $microsecs = $current - $prev_microsec_time_log;
    
  $total_microsec_time_log += $microsecs;

  dsm("$microsecs of $total_microsec_time_log microseconds", $label);
  $prev_microsec_time_log = $current;
}
/**
 * Function will build a simple node array for use in caching actions
 */
function cma_reservation_cache_node_availability_array($node) {
  return array('nid' => $node->nid,
	       'title' => $node->title,
	       'type' => $node->type,
	       'status' =>$node->reservations_default_availability,
	       );
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
 * Function will return an array of inventories for a form
 */
function cma_reservation_cache_inventories_array() {
  $inventories = array();

  $vid = variable_get('reservations_inventory_vid', 0); 
  $tree = taxonomy_get_tree($vid);
  foreach ($tree as $term) {
    $term = taxonomy_term_load($term->tid);
    $inventories[$term->tid] = $term->name;
  }
  asort($inventories);
  return $inventories;
}

/**
 * Function willl return an array of bucket categories 
 * (taxonomy terms applied to reservable content types
 */
function cma_reservation_cache_bucket_categories_array() {
  $vid = variable_get('reservations_grouping_vid', 0);
  $terms = taxonomy_get_tree($vid);
  $bucket_categories = array();
  foreach ($terms as $term) {
    $term = taxonomy_term_load($term->tid);
    $bucket_categories[$term->tid] = $term->name;
  }
  asort($bucket_categories);
  return $bucket_categories;
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
