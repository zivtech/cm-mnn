<?php
/**
 * Simply a util function that will call one of the "available" functions 
 * below
 */
function cma_reservation_cache_get_available_options($field_name,
						     $inventories,
						     $categories,
						     $content_types) {

  switch($field_name) {
  case 'inventories':
    $options = cma_reservation_cache_get_available_inventories();
    break;
  case 'categories':
    $options = cma_reservation_cache_get_available_categories($inventories);
    break;
  case 'content_types':
    $options = cma_reservation_cache_get_available_content_types($inventories,
								 $categories);
    break;
  case 'resources':
    $options = cma_reservation_cache_get_available_resources($inventories,
							     $categories,
							     $content_types);
    break;
  }
  return $options;
}
/**
 * Function will return the set of Inventories available filtered by any
 * inventory filters that are set
 */
function cma_reservation_cache_get_available_inventories() {
  //the array that we will return 
  $ret = array();
  $sql = "SELECT distinct inventory as id, inventory_name as value ";
  $sql .= " FROM cma_reservation_cache ";
  $sql .= " WHERE inventory > 0 " ;

  //run query and inventories into an array with the tid as the key
  $results = db_query($sql);
  while ($result = $results->fetchObject()) {
    $ret[$result->id] = $result->value;
  }
  asort($ret);
  return $ret;
}
/**
 * Function will return the set of Categories available filtered by any
 * inventory filters that are set
 */
function cma_reservation_cache_get_available_categories($inventories =
							array()) {
  //the array that we will return 
  $ret = array();
  $sql =
    "SELECT distinct bucket_category as id, bucket_category_name as value ";
  
  $sql .= " FROM cma_reservation_cache ";

  $first = TRUE;
  $args = array();
  //ADD WHERE CLAUSE OF INVENTORIES
  foreach($inventories as $key => $tid) {
    if ($first) {
      $first = FALSE;
      $sql .= " WHERE ";
    }
    else {
      $sql .= " OR ";
    }
    $sql .= " inventory = " . ":inventory_tid_" .$key;
    $args[":inventory_tid_".$key] = $tid;
  }

  //run query and categories into an array with the tid as the key
  $results = db_query($sql, $args);
  while ($result = $results->fetchObject()) {
    if ($result->id > 0) {
      $ret[$result->id] = $result->value;
    }
  }
  asort($ret);

  return $ret;
}
/**
 * Function will return the set of Content Types available filtered by any
 * inventory or category filters that are set
 */
function cma_reservation_cache_get_available_content_types($inventories =
							   array(),
							   $categories =
							   array()){
  //the array that we will return 
  $ret = array();
  $sql =
    "SELECT distinct content_type_machine_name as id, content_type as value ";
  
  $sql .= " FROM cma_reservation_cache ";

  $args = array();
  
  //ADD WHERE CLAUSE OF INVENTORIES
  if (!empty($inventories)) {
    $sql .= " WHERE (";
    $first = TRUE;  
    foreach($inventories as $key =>$tid) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " inventory = " . ":inventory_tid_".$key;
      $args[":inventory_tid_".$key] = $tid;
    }

    $sql .= ") ";
  }

  if (!empty($categories)) {
    if (empty($inventories)) {
      $sql .= " WHERE (";
    }
    else {
      $sql .= " AND (";
    }

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
  
  //run query and put the content types into an array with the machinename
  //as the key
  $results = db_query($sql, $args);
  while ($result = $results->fetchObject()) {
    $ret[$result->id] = $result->value;
  }
  asort($ret);

  return $ret;
}
/**
 * Function will return the set of Resorces available filtered by any
 * inventory, category, or type filters that are set
 */
function cma_reservation_cache_get_available_resources($inventories = array(),
						       $categories = array(),
						       $content_types=array()){
  //the array that we will return 
  $ret = array();
  $sql = "SELECT distinct item_nid as id, item_title as value ";
  $sql .= " FROM cma_reservation_cache ";

  $args = array();
    //ADD WHERE CLAUSE OF INVENTORIES
  if (!empty($inventories)) {
    $sql .= " WHERE (";
    $first = TRUE;  
    foreach($inventories as $key => $tid) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " inventory = " . ":inventory_" . $key;
      $args[":inventory_" . $key] = $tid;
    }

    $sql .= ") ";
  }
  
  if (!empty($categories)) {
    if (empty($inventories)) {
      $sql .= " WHERE (";
    }
    else {
      $sql .= " AND (";
    }

    $first = TRUE;
    //ADD WHERE CLAUSE OF CATEGORIES
    foreach($categories as $key => $tid) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR ";
      }
      $sql .= " bucket_category = " . ":bucket_category_".$key;
      $args[":bucket_category_".$key] = $tid;
    }
    $sql .= ") ";
  }
    
  if (!empty($content_types)) {
    if (empty($inventories) && empty($categories)) {
      $sql .= " WHERE (";
    }
    else {
      $sql .= " AND (";
    }

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

  //run query and put the resources into an array with the nid as the key
  $results = db_query($sql, $args);
  while ($result = $results->fetchObject()) {
    if ($result->id > 0 ){
      $ret[$result->id] = $result->value;
    }
  }
  asort($ret);
  return $ret;
}
/**
 * Function will return an array of retults form the cache table, each result
 * is a unique combination of item nid/slot id, content type, start, and end
 * time.
 *
 */
function cma_reservation_cache_get_item_time_slots($filter_values) {
  if (empty($filter_values)) {
    drupal_set_message('Error Code #329897 Filters are required', 'error');
    return array();
  }
  
  $item_time_slots = array();
  $fields = cma_reservation_cache_db_table_columns();

  //find our offset from UTC time which is what is in db
  //FIXME CALL FROM A FUNCTION THAT DEFINES A HOOK TO SET DIFFERENTLY IF
  //NECESSARY, OR MAYBE SET OFF THE RESERVATION FIELD ITSELF
  $offset = date_offset_get(new DateTime);
  
  //////////////////////////////////////////////////////////////////////////
  //START BUILDING THE SQL QUERY, START WITH THE FIELDS FOR THE SELECT
  $field_list = implode(', ', array_keys($fields));
  $sql = "SELECT " . $field_list . " ";
  if ($filter_values[CMA_RESERVATION_CACHE_BY_DAY]) {
    $sql .= ", date(from_unixtime(cache_slot_start_time)) as day ";
  }

  $sql .= " FROM cma_reservation_cache ";
  $where_and = " WHERE ";

  //////////////////////////////////////////////////////////////////////////
  //BUILD THE WHERE CLAUSE USING $filter_values
  $args = array();
  foreach($filter_values as $filter => $value) {
    if ($filter == CMA_RESERVATION_CACHE_FIRST_DAY) {
      if (!$value) {
	$cache_start_object = new DateTime();
	$hours = $cache_start_object->format('H');
	$minutes = $cache_start_object->format('i');
	$seconds = $cache_start_object->format('s');
	$cache_start_object->modify('-'.$hours.' hours');
	$cache_start_object->modify('-'.$minutes.' minutes');
	$cache_start_object->modify('-'.$seconds.' seconds');
      }
      else {
	$cache_start_object = new DateTime($value);
      }
      $cache_start_object->setTimeZone(new DateTimeZone('UTC'));
      $sql .= $where_and . " cache_slot_start_time >= :start";
      $where_and = " AND ";
      $args[':start'] = $cache_start_object->getTimestamp();
    }
    else if ($filter == CMA_RESERVATION_CACHE_LAST_DAY) {
      if (!$value && $cache_start_object) {
	$cache_end_object = clone $cache_start_object;
      }
      else if (!$value) {
	$cache_end_object = new DateTime();
	$hours = $cache_end_object->format('H');
	$minutes = $cache_end_object->format('i');
	$seconds = $cache_end_object->format('s');
	$cache_end_object->modify('-'.$hours.' hours');
	$cache_end_object->modify('-'.$minutes.' minutes');
	$cache_end_object->modify('-'.$seconds.' seconds');
	$cache_end_object->setTimeZone(new DateTimeZone('UTC'));
      }
      else {
	$cache_end_object = new DateTime($value);
	$cache_end_object->setTimeZone(new DateTimeZone('UTC'));
      }
      $cache_end_object->modify('+'.'1 day');
      
      $sql .= $where_and . " cache_slot_start_time < :end ";
      $where_and = " AND ";
      $args[':end'] = $cache_end_object->getTimestamp();
    }
    else if ($filter == CMA_RESERVATION_CACHE_BEGIN_HOUR) {
      if (!empty($value)) {
	$sql .= $where_and .
	  " hour(from_unixtime(cache_slot_start_time + $offset)) >= :hs ";
	$where_and = " AND ";
	$args[':hs'] = $value;
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_END_HOUR) {
      if (!empty($value)) {
	$sql .= $where_and .
	  " hour(from_unixtime(cache_slot_start_time + $offset)) < :he";
	$where_and = " AND ";
	$args[':he'] = $value;
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_DOW) {
      if (!empty($value)) {
	$sql .= $where_and .
	  " dayofweek(from_unixtime(cache_slot_start_time + $offset)) = :dow";
	$where_and = " AND ";
	$args[':dow'] = $value;
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_INVENTORY) {
      if (!empty($value)) {
	$sql .= $where_and . " (";
	$where_and = " AND ";
	$first = TRUE;
	foreach ($value as $key => $tid) {
	  if ($first) {
	    $first = FALSE;
	  }
	  else {
	    $sql .= " OR "; 
	  }
	  $sql .=  " inventory = " . ':inventory_tid_' . $key ." ";
	  $args[':inventory_tid_' . $key] = $tid;
	}
	$sql .= " ) ";
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_BUCKET_CATEGORY) {
      if (!empty($value)) {
	$sql .= $where_and . " (";
	$where_and = " AND ";
	$first = TRUE;
	foreach ($value as $key => $tid) {
	  if ($first) {
	    $first = FALSE;
	  }
	  else {
	    $sql .= " OR "; 
	  }
	  $sql .=  " bucket_category = " . ':cat_' . $key ." ";
	  $args[':cat_' . $key] = $tid;
	}
	$sql .= " ) ";
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_ITEM_TYPE) {
      if (!empty($value)) {
	$sql .= $where_and . " (";
	$where_and = " AND ";
	$first = TRUE;
	foreach ($value as $key => $name) {
	  if ($first) {
	    $first = FALSE;
	  }
	  else {
	    $sql .= " OR "; 
	  }
	  $sql .=  " content_type_machine_name = " . ':type_' . $key ." ";
	  $args[':type_' . $key] = $name;
	}
	$sql .= " ) ";
      }
    }
    else if ($filter == CMA_RESERVATION_CACHE_ITEM_NID) {
      if (!empty($value)) {
	$sql .= $where_and . " (";
	$where_and = " AND ";
	$first = TRUE;
	foreach ($value as $key => $name) {
	  if ($first) {
	    $first = FALSE;
	  }
	  else {
	    $sql .= " OR "; 
	  }
	  $sql .=  " item_nid = " . ':nid_' . $key ." ";
	  $args[':nid_' . $key] = $name;
	}
	$sql .= " ) ";
      }
    }
  }
  //////////////////////////////////////////////////////////////////////////
  //BUILD THE ORDER BY CLAUSE FOR THE FILTER QUERY
  $order_by = ' ORDER BY ';

  if ($filter_values[CMA_RESERVATION_CACHE_BY_DAY]) {
    $order_by .= " date(from_unixtime(cache_slot_start_time)), ";
  }
  
  $order_by .= " bucket_weight, bucket_category_name, content_type, " .
    "bucket_slot_id, item_nid, cache_slot_start_time ";
  
  $sql .= $order_by;

  //////////////////////////////////////////////////////////////////////////
  //EXECUTE THE SQL QUERY
  cma_reservation_cache_log_microsec('before cache table query');

  //FIXME ADD TO CONFIG SETTINGS FORM
  $max = variable_get('cma_reservation_cache_chart_max_cell_count', 100000);
  $sql .= " LIMIT $max ";
  
  //mnn_data_massage_log_query($sql, $args);
  $results = db_query($sql, $args);

  cma_reservation_cache_log_microsec('after cache table query');

  //FIXME, WHEN CMA_RESERVATION_CACHE_COL_SPAN FILTER IS SET, COMBINE
  //CELLS FOR RESERVATIONS THAT ARE LONGER THAN ONE CELL
  //foreach ($results as $result) {
  //  $item_time_slots[] = $result;
  //}
  $item_time_slots = $results;
  
  return $item_time_slots;
}
