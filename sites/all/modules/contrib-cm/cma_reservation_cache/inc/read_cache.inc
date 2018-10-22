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
  $variable_name = CMA_RESERVATION_CACHE_INVENTORY_OPTIONS;
  $options_cache = variable_get(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
  
  $ret = empty($options_cache[$variable_name]) ?
    array() : $options_cache[$variable_name];
  
  if (empty($ret)) {
    $ret = cma_reservation_cache_update_options_cache('inventory',
						      $options_cache,
						      $variable_name); 
  }
  return $ret;
}
/**
 * Function will return the set of Categories available filtered by any
 * inventory filters that are set
 */
function cma_reservation_cache_get_available_categories($inventories =
							array()) {


  $suffix .= cma_reservation_cache_options_suffix($inventories);

  $variable_name = CMA_RESERVATION_CACHE_CATEGORY_OPTIONS . $suffix;
  $options_cache = variable_get(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
  
  $ret = empty($options_cache[$variable_name]) ?
    array() : $options_cache[$variable_name];
  
  if (empty($ret)) {
    $ret = cma_reservation_cache_update_options_cache('category',
						      $options_cache,
						      $variable_name,
						      $inventories); 
  }
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
  $suffix .= cma_reservation_cache_options_suffix($inventories);
  $suffix .= cma_reservation_cache_options_suffix($categories);
  
  $variable_name = CMA_RESERVATION_CACHE_CONTENT_TYPE_OPTIONS . $suffix;
  $options_cache = variable_get(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());

  $ret = empty($options_cache[$variable_name]) ?
    array() : $options_cache[$variable_name];
  
  if (empty($ret)) {
    $ret = cma_reservation_cache_update_options_cache('content_type',
						      $options_cache,
						      $variable_name,
						      $inventories); 
  }
  return $ret;
}
/**
 * Function will return the set of Resorces available filtered by any
 * inventory, category, or type filters that are set
 */
function cma_reservation_cache_get_available_resources($inventories = array(),
						       $categories = array(),
						       $content_types=array()){

  $suffix .= cma_reservation_cache_options_suffix($inventories);
  $suffix .= cma_reservation_cache_options_suffix($categories);
  $suffix .= cma_reservation_cache_options_suffix($content_types);

  $variable_name = CMA_RESERVATION_CACHE_RESOURCE_OPTIONS . $suffix;
  $options_cache = variable_get(CMA_RESERVATION_CACHE_CHART_OPTIONS, array());
  $ret = empty($options_cache[$variable_name]) ?
    array() : $options_cache[$variable_name];

  if (empty($ret)) {
    $ret = cma_reservation_cache_update_options_cache('resource',
						      $options_cache,
						      $variable_name,
						      $inventories,
						      $categories,
						      $content_types); 
  }
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
  
  $fields = cma_reservation_cache_db_table_columns();

  $offset = cma_reservation_cache_get_offset();
  
  //////////////////////////////////////////////////////////////////////////
  //START BUILDING THE SQL QUERY, START WITH THE FIELDS FOR THE SELECT
  $field_list = implode(', ', array_keys($fields));
  $sql = "SELECT " . $field_list . " ";
  if (!empty($filter_values[CMA_RESERVATION_CACHE_SEPARATE_CHARTS])) {
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
      if (!empty($value) && $value != 24) {
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

  if ($filter_values[CMA_RESERVATION_CACHE_SEPARATE_CHARTS]) {
    $order_by .= " date(from_unixtime(cache_slot_start_time + $offset)), ";
  }
  
  $order_by .= " bucket_weight, bucket_category_name, content_type, " .
    "if(bucket_slot_id>0, bucket_slot_id, item_nid), cache_slot_start_time ";

  $sql .= $order_by;

  //////////////////////////////////////////////////////////////////////////
  //EXECUTE THE SQL QUERY
  //FIXME ADD TO CONFIG SETTINGS FORM
  $max = variable_get('cma_reservation_cache_chart_max_cell_count', 100000);
  $sql .= " LIMIT $max ";
  
  //mnn_data_massage_log_query($sql, $args);
  $results = db_query($sql, $args);
  cma_reservation_cache_log_time('Completed Cache Table Query');

  return $results;
}

/**
 * function returns all the fields from teh cache table
 */
function cma_reservation_cache_read_fields_from_db() {
  $cache_fields = array();
  //FIND THE FIELDS FOR THE BUCKET AND RESOURCE CACHE TABLES
  $sql = "show columns from {cma_reservation_cache}";
  $results = db_query($sql);
  $cache_fields = array();
  while ($result = $results->fetchObject()) {
    $cache_fields[] = $result->Field;
  }
  return $cache_fields;
}

/**
 * function returns the final start time of the cache
 */
function cma_reservation_cache_get_max_cache_date($item_nid = NULL) {
  
  //find last cache slot time
  $sql = "SELECT max(cache_slot_start_time) " .
    "FROM {cma_reservation_cache} ";

  if (!empty($item_nid)) {
    $sql .= "WHERE item_nid = :item_nid ";
  }

  return db_query($sql, array(':item_nid' => $item_nid))->fetchField();
}
/**
 * function returns the final start time of the cache
 */
function cma_reservation_cache_get_min_cache_date($item_nid = NULL) {
  
  //find last cache slot time
  $sql = "SELECT min(cache_slot_start_time) " .
    "FROM {cma_reservation_cache} ";

  if (!empty($item_nid)) {
    $sql .= "WHERE item_nid = :item_nid ";
  }

  return db_query($sql, array(':item_nid' => $item_nid))->fetchField();
}

/**
 * function returns an array of cache id's indexed by item nid plus timestamp
 * for resources and content type plus bucket slot id plus timestamp for 
 * buckets
 */
function cma_reservation_cache_get_cache_ids($cache_start_object,
					     $cache_end_object,
					     $content_types){

  $cache_ids = array();
  $args = array();
  
  $sql =
    "SELECT * " .
    "FROM cma_reservation_cache " .
    "WHERE  cache_slot_start_time >= :start " .
    "AND cache_slot_start_time < :end ";

  $args[':start'] = $cache_start_object->getTimestamp();
  $args[':end'] = $cache_end_object->getTimestamp();

  if (!empty($content_types)) {
    $sql .= " AND (";
    $first = TRUE;
    foreach ($content_types as $name => $data) {
      if ($first) {
	$first = FALSE;
      }
      else {
	$sql .= " OR "; 
      }
      $sql .=  " content_type_machine_name = " . ':type_' . $name ." ";
      $args[':type_' . $name] = $name;
    }
    $sql .= " ) ";
  }

  //////////////////////////////////////////////////////////////////////////
  //EXECUTE THE SQL QUERY
  $results = db_query($sql, $args);
    
  $counter = 0;
  while ($result = $results->fetchObject()) {
    $counter ++;
    $index = cma_reservation_slot_index($result->content_type_machine_name,
					$result->bucket_slot_id,
					$result->item_nid,
					$result->cache_slot_start_time);
    $cache_ids[$index] = $result;
    
  }
  
  $msg = 'Found ' . $counter . " existing slots in the db for time range.";
  cma_reservation_cache_log_time($msg);
  return $cache_ids;  
}
