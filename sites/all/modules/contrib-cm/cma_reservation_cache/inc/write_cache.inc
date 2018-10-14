<?php

/**
 * Function will update the cache after a reservable item has been created,
 * modified, or deleted
 */
function cma_reservation_cache_update_item_cache($node, $content_type, $op){
  cma_reservation_cache_log_microsec('starting update_item_cache');
  if ($content_type['reservations_type_setting'] == 'resource') {
    if ($op == 'delete') {
      $sql = "DELETE FROM {cma_reservation_cache} WHERE item_nid = :item_nid";
      db_query($sql, array(':item_nid' => $node->nid));
      cma_reservation_cache_log_microsec('deleted all rows for '.$node->title);
    }
    else if ($op == 'update') {
            $cache_start_object = $cache_end_object = NULL;
      cma_reservation_cache_set_range_date_objects($cache_start_object,
						   $cache_end_object);

      //fill the $content_types array with just this node's type
      $content_types = array($node->type=>$content_type);
      
      //fill the $items array with just this single node
      $items = array($result->nid =>
		     array('nid' => $node->nid,
			   'title' => $node->title,
			   'type' => $node->type,
			   'status' =>$node->reservations_default_availability,
			   ));	      

      cma_reservation_cache_log_microsec('about to get db rows');
      //get the array of data from the reservation base level
      $db_rows =
        cma_reservation_cache_get_rows_for_insert($cache_start_object,
						  $cache_end_object,
						  $content_types,
						  $items);

      cma_reservation_cache_log_microsec('got db rows');
      
      //pass the data from the base level to the function that groups
      //inserts and deletes and executes them in the db
      cma_reservation_cache_remove_and_replace_db_rows($db_rows);
      cma_reservation_cache_log_microsec('wrote db rows');
    }
    else if ($op == 'insert') {
      
    }
    //cache_time
    //content_type
    //content_type_machine_name
    //bucket_slot_id
    //cache_slot_status
    //item_status
    //item_title
    
  }
  else if ($content_type['reservations_type_setting'] == 'bucket') {

  }
      //get 


      /**
      $available = ($node->reservations_default_availability == 1) ? 1 : 0;

      $sql =
	"UPDATE cma_reservation_cache " .
	"SET cache_time = '" . date("Y-m-d H:i:s") . "', " .
	"bucket_slot_id = 0, " .
	"content_type_machine_name = :content_type_machine_name, " .
	"content_type = :content_type, " .
	"inventory = :inventory, " .
	"inventory_name = :inventory_name, " .
	"bucket_category = :bucket_category, " .
	"bucket_category_name = :bucket_category_name, " .
	"bucket_weight = :bucket_weight, " .
      	"item_nid = :item_nid, " .
	"item_title = :item_title, " .
	"item_status = :item_status, " .
	"cache_slot_status = CASE" .
	"  WHEN $available != " . RESERVATIONS_AVA_F .  "  THEN 0 " .
	"  WHEN reservation_nid is NULL THEN 1 " .
	"  WHEN (reservation_status = ";;

      */
      /**
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
      */





      

      /**
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

      */








      

}
/**
 * Function will read bucket weights and update the cache table
 */
function cma_reservation_cache_update_bucket_weights() {
  //BUCKET CATEGORIES  
  $vid = variable_get('reservations_grouping_vid', 0);
  $terms = taxonomy_get_tree($vid);
  $bucket_categories = array();
  foreach ($terms as $term) {
    $weight = variable_get($term->tid.'_rank_weight');
    if ($weight) {
      $sql =
	"UPDATE cma_reservation_cache SET bucket_weight = :weight" .
	" WHERE bucket_category = :category";
      $args = array(':weight' => $weight, ':category' => $term->tid);
      db_query($sql, $args);
    }
  }
}

/**
 * Function will query for current options for a filter
 */
function cma_reservation_cache_update_options_cache($type,
						    $options_cache,
						    $variable_name,
						    $inventories  = array(),
						    $categories = array(),
						    $content_types = array()) {
  $options = array();
  $args = array();
  $sql = '';
  
  //BUILD THE BASE SQL FOR THE QUERY
  if ($type == 'inventory') {
    $sql .= "SELECT distinct inventory as id, inventory_name as value ";
    $sql .= "FROM cma_reservation_cache ";
    $sql .= "WHERE inventory > 0 " ;
  }
  else if ($type == 'category') {
    $sql .= "SELECT distinct bucket_category as id, ";
    $sql .= "bucket_category_name as value ";
    $sql .= "FROM cma_reservation_cache ";
  }
  else if ($type == 'content_type') {
    $sql .= "SELECT distinct content_type_machine_name as id, ";
    $sql .= "content_type as value ";
    $sql .= "FROM cma_reservation_cache ";    
  }
  if ($type == 'resource') {
    $sql .= "SELECT distinct item_nid as id, item_title as value ";
    $sql .= "FROM cma_reservation_cache ";
  }

    
  /////////////////////////////////////////////////////////////////////////
  //ADD THE WHERE CLAUSE IF WE HAVE INVENTORIES, CATEGORIES OR CONTENT TYPES
  if (!empty($inventories)) {
    $sql .= "WHERE ";

    $first = TRUE;
    $sql .= "( ";
    //ADD WHERE CLAUSE OF INVENTORIES
    foreach($inventories as $key => $tid) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " inventory = " . ":inventory_tid_" .$key;
      $args[":inventory_tid_".$key] = $tid;
    }
    $sql .= ") ";
  }
  
  if (!empty($categories)) {
    $sql .= empty($inventories) ? " WHERE (" : " AND (";
    $first = TRUE;
    //ADD WHERE CLAUSE OF CATEGORIES
    foreach($categories as $key => $tid) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " bucket_category = " . ":bucket_category_" . $key;
      $args[":bucket_category_".$key] = $tid;
    }
    $sql .= ") ";
  }

  if (!empty($content_types)) {
    $sql .= empty($inventories) && empty($categories) ? " WHERE (" : " AND (";
    $first = TRUE;
    //ADD WHERE CLAUSE OF CONTENT_TYPES
    foreach($content_types as $key => $name) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " content_type_machine_name = " . ":type_".$key;
      $args[":type_".$key] = $name;
    }
    $sql .= ") ";
  }

  if (empty($inventories) && empty($categories)  && empty($content_types)
      && $type == 'resource') {
    $sql .= " WHERE bucket_slot_id = 0";
  }
  else if ($type == 'resource') {
    $sql .= " AND bucket_slot_id = 0";
  }
    
  /////////////////////////////////////////////////////////////////////////
  //run query and inventories into an array with the tid as the key
  $results = db_query($sql, $args);
  while ($result = $results->fetchObject()) {
    if (!empty($result->value)) {
      $options[$result->id] = $result->value;
    }
  }
  natcasesort($options);
  $options_cache[$variable_name] = $options;

  variable_set(CMA_RESERVATION_CACHE_CHART_OPTIONS, $options_cache);
  return $options;
  
}


/**
 * Function will take an array of db rows delete previous cache time slots 
 * and insert new cache time slots.
 *
 * It will also group deletes and queries by a configuration setting, will
 * default to 1K
 */
function cma_reservation_cache_remove_and_replace_db_rows($db_rows) {
  //find the db fields
  $cache_fields = cma_reservation_cache_read_fields_from_db();

  //remove the cache_id from fields as we won't insert a value for that column
  $insert_fields = array_diff($cache_fields, array('cache_id'));

  //start building the insert sql
  $insert_into = "INSERT INTO cma_reservation_cache ";
  $field_names = " ( " . implode(',', $insert_fields) . " ) VALUES ";
  $sql = $insert_into . $field_names;

  //start building the delete sql
  $delete_from = "DELETE FROM cma_reservation_cache ";
  $delete_sql = $delete_from;

  //args for our loop
  $is_first_row = TRUE;
  $args = $delete_args = array();
  $insert_counter = 0;
  //FIXME, ADD TO CONFIG PAGE
  $transaction_limit =
    variable_get('cma_reservation_cache_query_bundle_size', 1000);

  //loop over the rows we will be deleting and inserting 
  foreach($db_rows as $row) {
    //add to insert sql and delete sql depending on first row or subsequent row
    if ($is_first_row) {
      $is_first_row = FALSE;
      $delete_sql .= "WHERE ";
    }
    else {
      $sql .= ", ";
      $delete_sql .= "OR ";
    }
      
    $sql .= " ( " ;
    $is_first_field = TRUE;

    //loop through all of our fields in the cache table and pluck values out
    //of this $row
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

    //////////////////////////////////////////////////////////////////////
    //ADD A CLAUSE TO THE WHERE FOR THE DELETE 
    $delete_sql .= " ( " ;
    //if we have a bucket slot id, its acting as a bucket, so look up by
    //content type + bucket slot id
    if ($row['bucket_slot_id'] > 0) {
      $arg_name =  ':bucket_slot_id_' . $insert_counter;
      $delete_sql .= " bucket_slot_id = $arg_name ";
      $delete_args[$arg_name] = $row['bucket_slot_id'];
      
      $arg_name =  ':type_' . $insert_counter;
      $delete_sql .= " AND content_type_machine_name = $arg_name ";
      $delete_args[$arg_name] = $row['content_type_machine_name'];
    }
    //we are dealing with a resource so look up by item_nid
    else {
      $arg_name =  ':item_nid_' . $insert_counter;
      $delete_sql .= " item_nid = $arg_name";
      $delete_args[$arg_name] = $row['item_nid'];
    }
    //add start time to delete where clause
    $arg_name = ':start_' . $insert_counter;
    $delete_sql .= " AND cache_slot_start_time = $arg_name";
    $delete_args[$arg_name] =  $row['cache_slot_start_time'];

    //add end time to delete where clause
    $arg_name = ':end_' . $insert_counter;
    $delete_sql .= " AND cache_slot_end_time = $arg_name";
    $delete_args[$arg_name] = $row['cache_slot_end_time'];
    
    $delete_sql .= " ) ";

    $insert_counter ++;

    if ($insert_counter >= $transaction_limit) {
      //fixme, run query and requild sql
      break;
    }
  }
  
  cma_reservation_cache_log_microsec('after building sql');
  db_query($delete_sql, $delete_args);
  cma_reservation_cache_log_microsec('after delete');
  db_query($sql, $args);
  cma_reservation_cache_log_microsec('after insert');
}

/**
 * Function will clear out hte options array in teh variable table
 */
function cma_reservation_cache_reset_chart_options() {
  variable_set(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
}