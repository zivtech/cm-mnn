<?php

/**
 * Function will return an array of reservations for the filters passed in
 */
function cma_reservation_cache_reserved_timeslots($start_object,
						  $end_object,
						  $content_types = array(),
						  $items = array(),
						  &$reserved_timeslots,
						  &$cached_reservations) {
  //START BUILDING QUERY TO FIND ALL RESERVED ITEMS IN TIME FRAME
  $query =
    "SELECT d.entity_id as res_id, 
     rd.reservations_placeholder_nid as placeholder_nid,
     n.type as placeholder_type,  field_reservations_date_value as res_start,
     field_reservations_date_value2 as res_end,
     reservations_item_nid as item_nid ";
  
  //IF WE SET A PROJECT FIELD ON THE RESERVATION VIA THE CONFIG PAGE, ADD
  //IT TO SELECT CLAUSE
  $project_field = variable_get('cma_reservation_cache_project_field', '');
  if ($project_field) {
    $query .=
      ', pn.title as project_title, og.gid as project_nid ';
  }
  
  //START THE FROM CLAUSE
  $query .= 
    "FROM {field_data_field_reservations_date} d 
     INNER JOIN {reservations_reservation_detail} rd on rd.vid=d.revision_id
     INNER JOIN {node} n on n.nid=rd.reservations_placeholder_nid ";
  
  //IF WE SET A PROJECT FIELD ON THE RESERVATION VIA THE CONFIG PAGE, ADD
  //IT TO WHERE CLAUSE
  if ($project_field) {
    $query .=
      "LEFT JOIN {og_membership} og ON d.entity_id = og.etid ". 
      "LEFT JOIN {node} pn ON pn.nid=og.gid ";
  }
  
  //START THE WHERE CLAUSE WITH START AND END DATES
  $query .=
    "WHERE field_reservations_date_value2 >= :start
     AND field_reservations_date_value <= :end ";
  
  //START THE ARGS ARRAY
  $args = array(':start' => $start_object->format('Y-m-d H:i:s'),
		":end" => $end_object->format('Y-m-d H:i:s'));

  //IF WE PASSED IN CONTENT TYPES, FILTER BY THEM
  if (!empty($content_types)) {
    $query .= " AND ( ";
    $index = 0;
    $is_first = TRUE;
    foreach($content_types as $name=>$data) {
      if ($index > 0) {
	$query .= " OR ";
      }
      $index++;
      $arg_name = ':type_' . $index;
      $query .= " n.type = " . $arg_name;
      $args[$arg_name] = $name;
    }
    $query .= " ) ";
  }
  
  
  //IF WE PASSED IN ITEMS, FILTER BY THEM
  if (!empty($items)) {
    $index = 0;
    $query .= " AND ( ";
    foreach($items as $data) {
      if ($index > 0) {
	$query .= " OR ";
      }
      $index++;
      $arg_name = ':nid_' . $index;
      $query .= " reservations_item_nid = " . $arg_name;
      $args[$arg_name] = $data['nid'];
    }
    $query .= " ) ";
  }
  
  $query .= " ORDER BY field_reservations_date_value ";
  
  $results = db_query($query, $args);
  
  $reserved_timeslots = array();
  $cached_reservations = array();
  $increment = cma_reservation_cache_date_field_increment();

  //iterate over the db results for all reserved items during our time span
  while ($result = $results->fetchObject()) {
    //LOOK TO SEE IF WE HAVE FETCHED THIS RESERVATION BEFORE
    $reservation = !empty($cached_reservations[$result->res_id]) ?
      $cached_reservations[$result->res_id] : NULL;
    if (!$reservation) {
      $reservation = node_load($result->res_id);
      $cached_reservations[$result->res_id] = $reservation;
    }
    //IF FOR SOME REASON OUR RESERVATION NODE WAS DELETED, IGNORE
    if (!$reservation) {
      continue;
    }

    //set the date objects, adjusting times and timezones
    $date_field = $reservation->field_reservations_date[LANGUAGE_NONE][0];
    $start_object =
      new DateTime($date_field['value'], 
		   new DateTimeZone($date_field['timezone_db']));
    
    $start_object->setTimeZone(new DateTimeZone($date_field['timezone']));
    $prev_object = clone $start_object;

    $minutes = $prev_object->format('i');
    $seconds = $prev_object->format('s');

    if ($minutes > 0) {
      $prev_object->modify('-'.$minutes.' minutes');
    }
    if ($seconds > 0) {
      $prev_object->modify('-'.$seconds.' seconds');
    }

    $end_object =
      new DateTime($date_field['value2'], 
		   new DateTimeZone($date_field['timezone_db']));
    
    $end_object->setTimeZone(new DateTimeZone($date_field['timezone']));
    //fill our return array with reservations at every timeslot for the
    //resource item or bucket slot
    while ($prev_object < $end_object) {
      $next_object = clone $prev_object;
      $next_object->modify('+'.$increment.' minutes');
      if ($start_object < $next_object) {
	//make sure array is initialized for our timeslot
	$reserved_timeslots[$prev_object->getTimestamp()] =
	  !empty($reserved_timeslots[$prev_object->getTimestamp()]) ?
	  $reserved_timeslots[$prev_object->getTimestamp()] : array();
	$type = $result->placeholder_type;
	$reserved_timeslots[$prev_object->getTimestamp()][$type] =
	  !empty($reserved_timeslots[$prev_object->getTimestamp()][$type]) ?
	  $reserved_timeslots[$prev_object->getTimestamp()][$type] : array();

	$reserved_timeslots[$prev_object->getTimestamp()][$type][] = $result;
      }

      $prev_object = clone $next_object;
    }
  }
}

/**
 * Funciton will return an array of rows that will be inserted into the cache
 * table
 */
function cma_reservation_get_reservation_rows_for_insert($reservation) {
  //set the date objects, adjusting times and timezones
  $date_field = $reservation->field_reservations_date[LANGUAGE_NONE][0];
  $start_object =
    new DateTime($date_field['value'], 
		 new DateTimeZone($date_field['timezone_db']));
  
  $start_object->setTimeZone(new DateTimeZone($date_field['timezone']));
  
  $end_object =
    new DateTime($date_field['value2'], 
		 new DateTimeZone($date_field['timezone_db']));
  
  $end_object->setTimeZone(new DateTimeZone($date_field['timezone']));
 
  //find all the items tied to this reservations
  //START BUILDING QUERY TO FIND ALL RESERVED ITEMS IN TIME FRAME
  $query =
    "SELECT n.type, reservations_item_nid 
     FROM {node} res 
     INNER JOIN {reservations_reservation_detail} rd ON rd.vid =res.vid
     INNER JOIN {node} n on n.nid=rd.reservations_placeholder_nid 
     WHERE res.nid = :res_nid";
  $args = array(':res_nid' => $reservation->nid);
  $results = db_query($query, $args);

  $reservable_content_types = reservations_content_types();
  $items = $content_types = array();
  while ($result = $results->fetchObject()) {
    $content_types[$result->type] = $reservable_content_types[$result->type];
    $res_type = $content_types[$result->type]['reservations_type_setting'];
    if ($res_type == 'resource') {
      //fill the $items array with just this single resource node
      if (!empty($result->reservations_item_nid)) {
	$resource = node_load($result->reservations_item_nid);
	if ($resource) {
	  //FIXME THIS WON'T WORK CORRRCTLY WHEN RESERVATIONS HAVE BUCKETS
	  //AND RESOURCES, SO UNFORTUNATELY WILL END UP DEALING WITH ALL
	  //ITEMS IN THE CONTENT TYPE INSTEAD OF A SINGLE ONE. HOPEFULLY
	  //THIS IS MITIGATED BY THE FACT THAT MOST RESOURCE RESERVATIONS ARE
	  //SHORT.
	  //$items[$resource->nid] =
	  //  cma_reservation_cache_node_availability_array($resource);
	}
      }
      
    }	      
    else {
      //we keep $items empty so that we update the whole content type
    }
  }
  return cma_reservation_cache_get_rows_for_insert($start_object,
						   $end_object,
						   $content_types,
						   $items);
}

/**
 * Function will return an array of rows that will be inserted into 
 * the cache table
 */
function cma_reservation_cache_get_rows_for_insert($cache_start_object,
						   $cache_end_object,
						   $content_types,
						   $items = array()) {


  //prepare variables for loop
  $db_rows = array();
  $row_counter = 0;
  $bucket_slot_id = 0;
  $prev_content_type == NULL;
  $increment = cma_reservation_cache_date_field_increment();

  //an array of cache time slots that have a reservation and an array of
  //all of the cached reservations during the time period
  $reserved_timeslots = $cached_reservations = array();
  cma_reservation_cache_reserved_timeslots($cache_start_object,
					   $cache_end_object,
					   $content_types,
					   $items,
					   $reserved_timeslots,
					   $cached_reservations);
  $msg = "Found " . count($cached_reservations) . " reservations and " .
    count($reserved_timeslots) . " timeslots.";
  cma_reservation_cache_log_microsec($msg);


  //get array of cache id's for the relevant timeslots
  $cache_ids = cma_reservation_cache_get_cache_ids($cache_start_object,
						   $cache_end_object,
						   $content_types,
						   $items);
  //all of the fields on the reservations content type
  $reservation_fields =
    field_info_instances('node', 'reservations_reservation');

  $bucket_category_weights =
    cma_reservation_cache_get_bucket_category_weights();

  //just for logging, it will keep track of the potential new rows we skip
  //on updates because the new row is the same as what is already in db
  $skip_counter = 0;

  //if we don't have items because we are just dealing with buckets
  if (empty($items)) {
    $items = cma_reservation_cache_get_reservable_items($content_types);
  }
  
  //loop over all of the items/slots (no times though)
  foreach($items as $item) {
    //GET CONTENT TYPE INFO
    $content_type = $content_types[$item['type']];

    if (!empty($prev_content_type) && $prev_content_type != $item['type']) {
      $bucket_slot_id = 0;
    }
    $prev_content_type = $item['type'];
    
    $bucket_slot_id ++;
    
    $prev_object = clone $cache_start_object;

    //loop over each time block
    while ($prev_object < $cache_end_object) {
      $row_counter ++;
      
      $next_object = clone $prev_object;
      $next_object->modify('+'.$increment.' minutes');
      
      $prev_ts = $prev_object->getTimestamp();
      $next_ts = $next_object->getTimestamp();
      
      //LOOK UP ANY RESERVATIONS FOR THIS TIMESLOT AND CONTENT TYPE
      $reservation_result = NULL;
      $reservation_node = NULL;
      $reservations =
	!empty($reserved_timeslots[$prev_ts]) &&
	!empty($reserved_timeslots[$prev_ts][$item['type']]) ?
	$reserved_timeslots[$prev_ts][$item['type']] :
	array();

      if ($reservations) {	
	if ($content_type['reservations_type_setting'] == 'bucket') {
	  //POP AN ARRAY OFF THE STACK
	  $reservation_result =
	    array_shift($reserved_timeslots[$prev_ts][$item['type']]);
	  $reservation_node =
	    $cached_reservations[$reservation_result->res_id];
       	}
	else if($content_type['reservations_type_setting'] == 'resource') {
	  foreach($reservations as $reservation) {
	    if ($reservation->item_nid == $item['nid']) {
	      //this is a reservation against this resource, use as the
	      //result object that has values directly from db query
	      $reservation_result = $reservation;
	      //get cached reservation node object
	      $reservation_node =
		$cached_reservations[$reservation_result->res_id];
	    }
	  }
	}
      }
      
      //DONE FETCHING ALL DATA INTO ARRAYS, NOW TIME TO BUILD A DB ROW
      $db_row = array();
      $db_row['content_type_machine_name'] = $item['type'];
      
      $db_row['content_type'] = $content_type['type_name'];
      $db_row['cache_time'] = date("Y-m-d H:i:s");
      $db_row['cache_slot_start_time'] = $prev_ts;
      $db_row['cache_slot_end_time'] = $next_ts;
      $db_row['cache_slot_start_time_formatted'] =
	date('Y-m-d H:i:s',$prev_ts);
      $db_row['cache_slot_end_time_formatted'] =
	date('Y-m-d H:i:s',$next_ts);


      $tid = !empty($content_type['inventory_tids']) &&
	!empty($content_type['inventory_tids'][0]) ?
	$content_type['inventory_tids'][0] : NULL;

      $name = '';
      if ($tid) {
	$term = taxonomy_term_load($tid);
	if ($term) {
	  $name = $term->name;
	}
      }
      $db_row['inventory'] = $tid;
      $db_row['inventory_name'] = $name;

      $term = taxonomy_term_load($content_type['reservations_grouping']);
      if ($term) {
	$db_row['bucket_category'] = $content_type['reservations_grouping'];
	$db_row['bucket_category_name'] = $term->name;
	$db_row['bucket_weight'] =
	  $bucket_category_weights[$db_row['bucket_category']];
      }
      else {
	$db_row['bucket_category'] = 0;
	$db_row['bucket_category_name'] = '';
	//since no category, set to large number so its sorted last
	$db_row['bucket_weight'] = 1000;
      }
      
      if ($content_type['reservations_type_setting'] == 'bucket') {
	$db_row['bucket_slot_id'] = $bucket_slot_id;
		
	if ($reservation_result && $reservation_result->item_nid) {
	  $db_row['item_nid'] = $reservation_result->item_nid;
	}
	else {
	  $db_row['item_nid'] = NULL;
	}
      }
      else if($content_type['reservations_type_setting'] == 'resource') {
	$db_row['bucket_slot_id'] = 0;
	$db_row['item_nid'] = $item['nid'];
	$db_row['item_title'] = $item['title'];
	$db_row['item_status'] = $item['status'];
      }
      //get unique index for this cache item time slot, then see if we
      //have stored cache id for the slot
      $slot_index = cma_reservation_slot_index($item['type'],
					  $db_row['bucket_slot_id'],
					  $item['nid'],
					  $prev_object->getTimestamp());
      if (!empty($cache_ids[$slot_index])) {
	$db_row['cache_id'] = $cache_ids[$slot_index]->cache_id;
      }

      if ($reservation_node) {
	$db_row['reservation_nid'] = $reservation_node->nid;
	$db_row['reservation_uid'] = $reservation_node->uid;
	$db_row['reservation_user_name'] = $reservation_node->name;

	
	//FIXME SET RESERVATION GROUP ID FOR REAL
	$db_row['reservation_group_id'] = 0;

	//set status code and name
	$status_code = $reservation_node->reservations_reservation_status;
	$db_row['reservation_status'] = $status_code;
	$db_row['reservation_status_name'] =
	  reservations_record_status($status_code);
	
	$skip_types = array('viewfield',);
	
	$skip_names = array('field_reservations_inventory');
	foreach ($reservation_fields as $field_name => $value) {
	  $field_info = field_info_field($field_name);
	  if (in_array($field_info['type'], $skip_types)) {
	    continue;
	  }
	  if (in_array($field_name, $skip_names)) {
	    continue;
	  }
	  
	  switch ($field_info['type']) {
	  case 'entityreference':
	    $index = 'target_id';
	    break;
	  case 'taxonomy_term_reference':
	    $index = 'tid';
	    break;
	  case 'computed':
	  case 'datetime':
	  case 'number_integer':
	  case 'text':
	  case 'text_long':
	  default:
	    $index = 'value';
	    break;
	  }
	  
	  if ($field_info['type'] == 'datetime' &&
	      !empty($field_info['columns']['value']) &&
	      !empty($field_info['columns']['value2'])) {
	    $db_row[$field_name."_start"] =
	      cma_reservations_cache_get_single_field_value($reservation_node,
							    $field_name,
							    $index);
	    $db_row[$field_name."_end"] = 
	      cma_reservations_cache_get_single_field_value($reservation_node,
							    $field_name,
							    'value2');
	  }
	  else {
	    $db_row[$field_name] = 
	      cma_reservations_cache_get_single_field_value($reservation_node,
							    $field_name,
							    $index);
	    
	  }
	}
      }
      
      //IF BAD ITEM STATUS ITEM IS UNAVAIALABLE
      if ($item['status'] != RESERVATIONS_AVA_F) {
	$db_row['cache_slot_status'] = 0;
      }
      //WE HAVE A RESERVATION IN THIS TIME SLOT
      else if ($reservation_result) {
	if ($reservation_result->reservations_reservation_status ==
	    RESERVATIONS_STATUS_UNCONFIRMED) {
	  if (variable_get('reservations_ignore_unconfirmed_reservations')) {
	    $db_row['cache_slot_status'] = 1;
	  }
	  else {
	    $db_row['cache_slot_status'] = 0;
	  }
	}
	else if ($reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_PENDING ||
		 $reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_CHECKED_OUT) {
	  $db_row['cache_slot_status'] = 0;
	}
	else if ($reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_CHECKED_IN ||
		 $reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_CANCELLED ||
		 $reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_DENIED ||
		 $reservation_result->reservations_reservation_status ==
		 RESERVATIONS_STATUS_NO_SHOW) {
	  $db_row['cache_slot_status'] = 1;
	}
	//UNKNOWN RESERVATION STATUS
	else {
	  $db_row['cache_slot_status'] = 1;
	}
      }
      //WE HAVE A GOOD ITEM STATUS AND NO RESERVATION SO ITS AVAILABLE
      else {
	$db_row['cache_slot_status'] = 1;
      }
      
      //set project_nid and project_title using title field set in settings
      $project_field_name =variable_get('cma_reservation_cache_project_field');
      if ($project_field_name) {
	$db_row['project_nid'] = $reservation_result->project_nid;
	$db_row['project_title'] = $reservation_result->project_title;
      }
      
      $row_changed =
	cma_reservation_cache_row_changed($db_row, $cache_ids[$slot_index]);
      //only store the row if it has changed compared to what is already in
      //the cache db table
      if ($row_changed) {
	$db_rows[] = $db_row;
      }
      else {
	//just for logging
	$skip_counter ++;
      }

      $prev_object = clone $next_object;
    } //done looping through the item for all time blocks
  }

  //THIS IS JUST LOGGING
  $insert_update_count = count($db_rows);
  $msg = "Inserted/Updated " . $insert_update_count ." rows and ignored " .
    $skip_counter . " unchanged rows. Total(" .
    ($insert_update_count + $skip_counter) . ")";

  cma_reservation_cache_log_microsec($msg);
  return $db_rows;
}

/**
 * Function will return an array of nodes of a set of content types
 */
function cma_reservation_cache_get_reservable_items($content_types = array()){
  $args = array();
  //start query to find reservable items
  $query =
    "SELECT n.nid, n.title, n.type, rin.reservations_default_availability 
     FROM {node} n
     INNER JOIN {reservations_reservation_item_node} rin ON n.vid = rin.vid";

  //limit to actual items, not placeholder nodes used for reservations
  $query .= " WHERE rin.reservations_sub_type = :reservations_sub_type ";
  $args[':reservations_sub_type'] = RESERVATIONS_SUB_TYPE_ITEM;

  //if we have passed in content types, limit which content types we fetch
  //items for
  if (!empty($content_types)) {
    $query .= " AND (";
    $index = 0;
    foreach($content_types as $type=>$data) {
      if ($index > 0) {
	$query .= " OR ";
      }
      $var_name = ":content_type_" . $index;
      $query .= "n.type = " . $var_name . " ";
      $args[$var_name] = $type;
      $index++;
    }
    $query .= ") ";
  }
  else {
    $content_types = reservations_content_types();
  }
  $query .= "ORDER BY n.type ASC, n.nid DESC";
  $results = db_query($query, $args);

  $reservable_content_type_items = array();

  while ($result = $results->fetchObject()) {
    if (!$content_types[$result->type]) {
      continue;
    }
    $reservable_content_type_items[$result->nid] =
      array('nid' => $result->nid,
	    'title' => $result->title,
	    'type' => $result->type,
	    'status' => $result->reservations_default_availability,
	    );
  }

  //logging
  $msg = 'Found ' . count($reservable_content_type_items) . ' items for ' .
    "relevant content types";
  cma_reservation_cache_log_microsec($msg);
 
  return $reservable_content_type_items;
}