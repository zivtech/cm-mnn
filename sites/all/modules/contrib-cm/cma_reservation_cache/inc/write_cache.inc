<?php

/**
 * Function will delete all rows in the cache table for the item
 */
function cma_reservation_cache_delete_item_time_slots($node) {
  $sql = "DELETE FROM {cma_reservation_cache} WHERE item_nid = :item_nid";
  db_query($sql, array(':item_nid' => $node->nid));
  cma_reservation_cache_log_time('deleted all rows for '.$node->title);
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
  //logging
  cma_reservation_cache_log_query($sql, $args);
  
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
 * default to 5K
 */
function cma_reservation_cache_insert_and_update_db_rows($db_rows) {
  //find the db fields
  $cache_fields = cma_reservation_cache_read_fields_from_db();
  
  //start building the insert sql
  $insert_into = "INSERT INTO cma_reservation_cache ";
  //remove the cache_id from fields as we won't insert a value for that column
  $insert_fields = array_diff($cache_fields, array('cache_id'));
  $insert_field_names = " ( " . implode(',', $insert_fields) . " ) VALUES ";
  $insert_sql = $insert_into . $insert_field_names;

  //start building the update sql
  $update_fields = $cache_fields;
  $update_field_names = " ( " . implode(',', $cache_fields) . " ) VALUES ";
  $update_sql = $insert_into . $update_field_names;
  $duplicate_key_update =
    cma_reservation_cache_duplicate_key_update($insert_fields);

  
  //args for our loop
  $is_first_insert_row = $is_first_update_row = TRUE;
  $insert_args = $update_args = array();
  $insert_counter = $update_counter = 0;
  //FIXME, ADD TO CONFIG PAGE
  $transaction_limit =
    variable_get('cma_reservation_cache_query_bundle_size', 5000);

  //loop over the rows we will be deleting and inserting 
  foreach($db_rows as $row) {
    if (empty($row['cache_id'])) {
      cma_reservation_cache_add_values_to_query($row,
						$insert_fields,
						$is_first_insert_row,
						$insert_sql,
						$insert_args,
						$insert_counter);
    }
    else {
      cma_reservation_cache_add_values_to_query($row,
						$update_fields,
						$is_first_update_row,
						$update_sql,
						$update_args,
						$update_counter);
    }
    if ($update_counter >= $transaction_limit) {
      $update_sql .= $duplicate_key_update;
      db_query($update_sql, $update_args);
      //logging
      cma_reservation_cache_log_time('Updated ' . $update_counter . " rows.");

      //reset counters and queries
      $is_first_update_row = TRUE;
      $update_counter = 0;
      $update_sql = $insert_into . $update_field_names;
      $update_args = array();
      continue;
    }
    else if ($insert_counter >= $transaction_limit) {
      db_query($insert_sql, $insert_args);
      //logging
      cma_reservation_cache_log_time('Inserted ' . $insert_counter ." rows.");
      
      //reset counters and queries
      $is_first_insert_row = TRUE;
      $insert_counter = 0;
      $insert_sql = $insert_into . $insert_field_names;
      $insert_args = array();
      continue;      
    }
  }//end of loop

  if ($update_counter > 0) {
    $update_sql .= $duplicate_key_update;
    db_query($update_sql, $update_args);
    //logging
    //cma_reservation_cache_log_query($update_sql, $update_args);
    cma_reservation_cache_log_time('Ran final update of ' . $update_counter .
				   " rows.");
  }

  if ($insert_counter > 0) {
    db_query($insert_sql, $insert_args);
    //logging
    //cma_reservation_cache_log_query($insert_sql, $insert_args);
    cma_reservation_cache_log_time('Ran final insert of ' . $insert_counter .
				   " rows.");
  }
}
/**
 * Function will clear out hte options array in teh variable table
 */
function cma_reservation_cache_reset_chart_options() {
  variable_set(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
}

/**
 * Function will remove a content type from the cache table
 */
function cma_reservation_cache_remove_content_type($type) {
  $sql =
    "delete from {cma_reservation_cache} " .
    "where content_type_machine_name = :type";

  db_query($sql, array(':type' => $type));
}