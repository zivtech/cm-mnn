<?php

/**
 * Function will return an array of retults form the cache table, each result
 * is a unique combination of item nid/slot id, content type, start, and end
 * time.
 *
 */
function cma_reservation_cache_get_item_time_slots($filter_values) {
  cma_reservation_cache_log_microsec('get_item_time_slots()');
  if (empty($filter_values)) {
    drupal_set_message('Error Code #329897 Filters are required', 'error');
    return array();
  }
  
  $item_time_slots = array();
  $fields = cma_reservation_cache_db_table_columns();
  
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
      $args[':end'] = $cache_end_object->getTimestamp();
    }
    $where_and = " AND ";
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
  $results = db_query($sql, $args);
  cma_reservation_cache_log_microsec('after cache table query');

  //FIXME, WHEN CMA_RESERVATION_CACHE_COL_SPAN FILTER IS SET, COMBINE
  //CELLS FOR RESERVATIONS THAT ARE LONGER THAN ONE CELL
  //foreach ($results as $result) {
  //  $item_time_slots[] = $result;
  //}
  $item_time_slots = $results;

  dsm("Found " . count($item_time_slots) . " item time slots.");
  cma_reservation_cache_log_microsec('end of get_item_time_slots()');
  
  return $item_time_slots;
}
