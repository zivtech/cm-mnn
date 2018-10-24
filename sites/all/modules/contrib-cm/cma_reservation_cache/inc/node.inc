<?php
/**
 * Function called from hook_insert, hook_update, and hook_delete, it checks to
 * see if it has been run yet (insert and delete hooks fire multiple times 
 * somtimes) and if it hasn't, it checks content type and status to see if
 * we need to go update the cache for our item and its content type
 */
function cma_reservation_cache_node_postsave($node, $op) {
  $content_types = reservations_content_types();
  if (!empty($content_types[$node->type]) && $node->status > 0 ) {
    $msg = '------------Started postsave for reservable item: ' . $node->title;
    cma_reservation_cache_log_time($msg);
    cma_reservation_cache_update_item_cache($node, $op,
					    $content_types[$node->type]);
    cma_reservation_cache_log_time('Finished postsave for reservable item');
  }
  else if ($node->type == 'reservations_reservation') {
    $msg = '-----------Started postsave for a reservation.';
    cma_reservation_cache_log_time($msg);
    $rows = cma_reservation_get_reservation_rows_for_insert($node);
    cma_reservation_cache_insert_and_update_db_rows($rows);
    cma_reservation_cache_log_time('Finished postsave for reservation.');
  }
}

/**
 * Insert hook for nodes, will see if item is of a reservable content type
 * then it will update the cache table for future rows
 */
function cma_reservation_cache_node_insert($node) {
  cma_reservation_cache_node_postsave($node, 'insert');
}


/**
 * Implements hook_node_update().
 */
function cma_reservation_cache_node_update($node) {
  cma_reservation_cache_node_postsave($node, 'update');
}

/**
 * Implements hook_node_delete()
 */
function cma_reservation_cache_node_delete($node) {
  cma_reservation_cache_node_postsave($node, 'delete');
}

/**
 * Function will update the cache after a reservable item has been created,
 * modified, or deleted
 */
function cma_reservation_cache_update_item_cache($node, $op,
						 $content_type = array()){
  //if no content type passed in, set it from node
  if (empty($content_type)) {
    $content_types = reservations_content_types();
    if (!empty($content_types[$node->type])) {
      $content_type = $content_types[$node->type];
    }
  }
  
  //this will be either "bucket" or "resource"
  $res_type = $content_type['reservations_type_setting'];
  
  if ($res_type == 'resource' && $op == 'delete') {
    cma_reservation_cache_delete_item_time_slots($node);
    return;
  }

  //set the start and end date objects of the range of cache we will update
  $cache_start_object = $cache_end_object = NULL;
  cma_reservation_cache_set_range_date_objects($cache_start_object,
					       $cache_end_object);

  //fill the $content_types array with just this node's type
  $content_types = array();
  $content_types[$node->type] = $content_type;

  $items = array();
  if ($res_type == 'resource') {
    //fill the $items array with just this single resource node
    $items[$node->nid] = cma_reservation_cache_node_availability_array($node);
  }	      
  else {
    //we keep $items empty so that we update the whole content type
  }
  
  //get the array of data from the reservation base level
  $db_rows = cma_reservation_cache_get_rows_for_insert($cache_start_object,
						       $cache_end_object,
						       $content_types,
						       $items);
  
  //pass the data from the base level to the function that groups
  //inserts and delete statements and then executes them in the db
  cma_reservation_cache_insert_and_update_db_rows($db_rows);
}
/**
 * Implements hook_node_type_insert().
 */
function cma_reservation_cache_node_type_insert($info) {
  cma_reservation_cache_node_type_postsave($info);
}

/**
 * Implements hook_node_type_update().
 */
function cma_reservation_cache_node_type_update($info) {
  cma_reservation_cache_node_type_postsave($info);
}

/**
 * Function called by both update and insert hooks for a content type
 */
function cma_reservation_cache_node_type_postsave($info) {
  $type = $info->type;

  $variables = array(
    'reservations_max_hours_per_reservation',
    'reservations_allow_overnight',
    'reservations_allow_weekends',
    'reservations_late_fee_per_hour',
    'reservations_rate_per_hour',
    'reservations_fee_free_hours',
    'reservations_status',
    'reservations_spare_items',
    'reservations_min_cancel_hours',
    'reservations_autocheckout',
    'reservations_autocheckin',
    'reservations_selfcheckout',
    'reservations_grouping',
    'reservations_active_status',
    'reservations_auto_assign_bucket_item',
  );

  $info->inventory_tids = reservations_inventory_get_inventory_tids($type);  
  $info->reservations_type_setting = reservations_type_setting($info->type);

  foreach ($variables as $variable) {
    $info->{$variable} = variable_get($variable . '_' . $type, 0);
  }
  
  $content_type = (array) $info;
  $content_type['type_name'] = $content_type['name'];

  $msg =
    '------------Started postsave for content type: ' . $content_type['name'];
  
  cma_reservation_cache_log_time($msg);
  
  if ($content_type) {
    $content_types = array($info->type => $content_type);
  
    //set the start and end date objects of the range of cache we will update
    $cache_start_object = $cache_end_object = NULL;
    cma_reservation_cache_set_range_date_objects($cache_start_object,
						 $cache_end_object);
  
    //get the array of data from the reservation base level
    $db_rows = cma_reservation_cache_get_rows_for_insert($cache_start_object,
							 $cache_end_object,
							 $content_types,
							 $items);
  
    //pass the data from the base level to the function that groups
    //inserts and delete statements and then executes them in the db
    cma_reservation_cache_insert_and_update_db_rows($db_rows);
  }
  else {
    dsm("hi mom");
    cma_reservation_cache_remove_content_type($type);
  }
}