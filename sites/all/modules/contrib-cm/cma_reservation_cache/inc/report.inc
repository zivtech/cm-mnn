<?php


/**
 * Form gives information on cache and tools to reload it
 * @param type $form
 * @return $form
 */
function cma_reservation_cache_report($form) {
  $form = array();

  //number of content types
  //number of bucket slots
  //number of resource items
  //earliest date
  //latest date
  $form['cache']['start_date'] =
    array(
	  '#title' => 'Start of Cache Refresh',
	  '#type' => 'date_popup',
	  '#date_format' => "Y-m-d",
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => date("Y-m-d"),
	  '#description' => '',
	  );
  $form['cache']['end_date'] =
    array(
	  '#title' => 'End of Cache Refresh',
	  '#type' => 'date_popup',
	  '#date_format' => "Y-m-d",
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => date("Y-m-d"),
	  '#description' => '',
	  );

  $form['submit'] =
    array('#type' => 'submit',
	  '#value' => t('Clear and Reload Cache'),
	  );
  return $form;
}

/**
 *
 */
function cma_reservation_cache_report_submit($form, $form_state) {
  //get start and end from the submitted form values
  $cache_start = $form_state['values']['start_date'] . " 00:00:00";
  $cache_end = $form_state['values']['end_date'] . " 23:59:59";
  $cache_start_object = new DateTime($cache_start);
  $cache_end_object = new DateTime($cache_end);

  $content_types = reservations_content_types();
  $bucket_category_weights=cma_reservation_cache_get_bucket_category_weights();

  $reservation_fields =
    field_info_instances('node', 'reservations_reservation');

  $increment = $reservation_fields['field_reservations_date']
    ['widget']['settings']['increment'];
  
  //FIND THE FIELDS FOR THE BUCKET AND RESOURCE CACHE TABLES
  $sql = "show columns from cma_reservation_cache";
  $results = db_query($sql);
  $cache_fields = array();
  while ($result = $results->fetchObject()) {
    $cache_fields[] = $result->Field;
  }
  //////////////////////////////////////////////////////////////////////////
  //FILL THE $reservable_content_type_items ARRAY
  
  //GRAB ALL NODES WHOSE CONTENT TYPES ARE RESERVABLE AND IS AN ACTUAL ITEM
  //NOT A PLACEHOLDER FOR A RESERVATION
  $query =
    "SELECT n.nid, n.title, n.type, rin.reservations_default_availability 
     FROM {node} n
     INNER JOIN {reservations_reservation_item_node} rin ON n.vid = rin.vid
     WHERE rin.reservations_sub_type = :reservations_sub_type
     ORDER BY n.type ASC, n.nid ASC

  ";

  //   ORDER BY n.type ASC
  //  ";
  // FIXME, MAKE SURE ABOVE IS GONE AFTER TESTING
  // AND n.type='lowel_o110_p_'
  // AND n.type='firehouse_express_studios_fh_'
  //AND n.type = 'headphones_p_'
  // LIMIT 1
  
  $args = array(':reservations_sub_type' => RESERVATIONS_SUB_TYPE_ITEM);
  $results = db_query($query, $args);

  $reservable_content_type_items = array();
  dsm(date('H:i:s'), 'start of item discover loop');

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
  dsm($reservable_content_type_items, 'reservable content type items');

  //////////////////////////////////////////////////////////////////////////
  //FILL THE $reserved_timeslots ARRAY
  $project_field = variable_get('cma_reservation_cache_project_field', '');
    
  $query =
    "SELECT d.entity_id as res_id, 
     rd.reservations_placeholder_nid as placeholder_nid,
     n.type as placeholder_type,  field_reservations_date_value as res_start,
     field_reservations_date_value2 as res_end,
     reservations_item_nid as item_nid ";

  if ($project_field) {
    $query .=
      ', pn.title as project_title, og.gid as project_nid ';
  }
  
  $query .= 
    "FROM field_data_field_reservations_date d 
     INNER JOIN reservations_reservation_detail rd on rd.vid=d.revision_id
     INNER JOIN node n on n.nid=rd.reservations_placeholder_nid ";

  if ($project_field) {
    $query .=
      "LEFT JOIN {og_membership} og ON d.entity_id = og.etid ". 
      "LEFT JOIN node pn ON pn.nid=og.gid ";
  }

  $query .=
    "WHERE field_reservations_date_value2 >= :start
     AND field_reservations_date_value <= :end ";

  //FIXME ONLY HERE FOR TESTING
  //$query .= " and n.type = 'lowel_o110_p_' ";

  $query .= "ORDER BY field_reservations_date_value ";
  $args = array(':start' => $cache_start,":end" => $cache_end);;

  $results = db_query($query, $args);

  $reserved_timeslots = array();
  $cached_reservations = array();
  dsm(date('H:i:s'), 'start of reservation loop');

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

    while ($prev_object < $end_object) {
      $next_object = clone $prev_object;
      $next_object->modify('+'.$increment.' minutes');
      if ($start_object < $next_object) {
	//make sure array is initialized for our timeslot
	$reserved_timeslots[$prev_object->getTimestamp()] =
	  $reserved_timeslots[$prev_object->getTimestamp()] ?
	  $reserved_timeslots[$prev_object->getTimestamp()] : array();
	$type = $result->placeholder_type;
	$reserved_timeslots[$prev_object->getTimestamp()][$type] =
	  $reserved_timeslots[$prev_object->getTimestamp()][$type] ?
	  $reserved_timeslots[$prev_object->getTimestamp()][$type] : array();

	$reserved_timeslots[$prev_object->getTimestamp()][$type][] = $result;
      }

      $prev_object = clone $next_object;
    }
  }
  dsm($reserved_timeslots, 'reserved_timeslots');
  dsm($cached_reservations, 'reservations');

  //FIXME ONLY HERE FOR TESTING
  foreach($reserved_timeslots as $timestamp_tmp => $slot_tmp) {
    //dsm($slot_tmp, date('Y-m-d H:i:s',$timestamp_tmp).' *  '.$timestamp_tmp);
  }
  //////////////////////////////////////////////////////////////////////////
  //FILL THE $timeslots ARRAY WITH ALL TIMESLOTS
  $cache_start_object = new DateTime($cache_start);
  $cache_end_object = new DateTime($cache_end);

  dsm(date('H:i:s'), 'start of item loop');
  $row_counter = 0;
  $bucket_slot_id = 0;
  $prev_content_type == NULL;
  //loop over all of the items/slots (no times though)
  foreach($reservable_content_type_items as $item) {
    //GET CONTENT TYPE INFO
    $content_type = $content_types[$item['type']];

    if (!empty($prev_content_type) && $prev_content_type != $item['type']) {
      $bucket_slot_id = 0;
    }
    $prev_content_type = $item['type'];
    
    $bucket_slot_id ++;
    
    $prev_object = clone $cache_start_object;

    //this will store all the db rows for a time slot for all content types
    //it gets refreshed every new 
    $db_rows = array();

    //loop over each time block
    while ($prev_object < $cache_end_object) {
      $row_counter ++;
      
      $next_object = clone $prev_object;
      $next_object->modify('+'.$increment.' minutes');
      
      $prev_ts = $prev_object->getTimestamp();
      $next_ts = $next_object->getTimestamp();
      
      /**
      dsm($prev_object->format('Y-m-d H:i:s'), 'prev');
      dsm($next_object->format('Y-m-d H:i:s'), 'next');
      dsm($prev_ts, 'prev');
      dsm($next_ts, 'next');
      **/
      
      
      //LOOK UP ANY RESERVATIONS FOR THIS TIMESLOT AND CONTENT TYPE
      $reservation_result = NULL;
      $reservation_node = NULL;
      $reservations =
	!empty($reserved_timeslots[$prev_ts]) &&
	!empty($reserved_timeslots[$prev_ts][$item['type']]) ?
	$reserved_timeslots[$prev_ts][$item['type']] :
	array();

      //dsm($reservations, 'find reservations?');
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
	/**
	dsm($content_type, 'ct');
	dsm($item, 'item');
	dsm($reservation_result, 'result');
	dsm($reservation_node, 'reservation');
	**/
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
      //dsm($reservation_node, 'reservation node');
      //  dsm($reservation_result, 'reservation db result');

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
      $db_rows[] = $db_row;
      $prev_object = clone $next_object;
    } //done looping through the item for all time blocks

    $table = '';
    $insert_fields = array_diff($cache_fields, array('cache_id'));

    $sql ="INSERT INTO cma_reservation_cache ";

    $sql .= " ( " . implode(',', $insert_fields) . " ) VALUES ";

    $is_first_row = TRUE;
    $args = array();
    $insert_counter = 0;

    foreach($db_rows as $row) {
      if ($is_first_row) {
	$is_first_row = FALSE;
      }
      else {
	$sql .= ", ";
      }
      
      $sql .= " ( " ;
      $is_first_field = TRUE;
      foreach($insert_fields as $key=>$value) {
	if ($is_first_field) {
	  $is_first_field = FALSE;
	}
	else {
	  $sql .= ", ";
	}
	$arg_name =  ':' . $value . '_' . $insert_counter;
	$sql .= $arg_name;
	$args[$arg_name] = $row[$value];
      }
      $sql .= " ) ";
      $insert_counter ++;
    }

    //dsm($sql, 'sql');
    //dsm($args, 'args');
    $delete_sql =
      "DELETE FROM cma_reservation_cache " .
      "WHERE cache_slot_start_time >= :start " .
      "AND cache_slot_start_time <= :end ";

    if ($db_row['bucket_slot_id'] > 0) {
      $delete_sql .= " AND bucket_slot_id = :bucket_slot_id ";
      $delete_sql .= " AND content_type_machine_name = :type ";
      $delete_args = array(':bucket_slot_id' => $db_row['bucket_slot_id'],
			   ':type' => $db_row['content_type_machine_name']);
    }
    else {
      $delete_sql .= " AND item_nid = :item_nid";
      $delete_args = array(':item_nid' => $db_row['item_nid']);
    }
    $delete_args[':start'] = $cache_start_object->getTimestamp();
    $delete_args[':end'] = $cache_end_object->getTimestamp();

    //delete all timeblocks within time range for this item/bucket slot
    db_query($delete_sql, $delete_args);

    //add all timeblocks within time range for this item/bucket slot
    db_query($sql, $args);
    dsm(date('H:i:s'), 'after insert');
  }
  dsm(date('H:i:s'), 'end of item loop');
  dsm($row_counter, 'number of rows');

  //FIXME ONLY HERE FOR TESTING
  //db_query("select dfjd from dfjdlk");
  
}
