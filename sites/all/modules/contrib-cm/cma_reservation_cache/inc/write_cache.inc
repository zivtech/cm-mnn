<?php

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
 * Function will clear out hte options array in teh variable table
 */
function cma_reservation_cache_reset_chart_options() {
  variable_set(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
}