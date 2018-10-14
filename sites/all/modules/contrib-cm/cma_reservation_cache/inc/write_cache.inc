<?php

/**
 * Function will delete all rows in the cache table for the item
 */
function cma_reservation_cache_delete_item_time_slots($node) {
  $sql = "DELETE FROM {cma_reservation_cache} WHERE item_nid = :item_nid";
  db_query($sql, array(':item_nid' => $node->nid));
  cma_reservation_cache_log_microsec('deleted all rows for '.$node->title);
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
    variable_get('cma_reservation_cache_query_bundle_size', 10000);

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