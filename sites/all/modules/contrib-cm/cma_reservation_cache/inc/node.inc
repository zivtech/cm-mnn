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
    cma_reservation_cache_log_microsec('start postsave for a reservable item');
    cma_reservation_cache_update_item_cache($node, $op,
					    $content_types[$node->type]);
    cma_reservation_cache_log_microsec('finished postsave for reservble item');
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
    //get all items for this content types, so we can redo bucket slots
    $items = cma_reservation_cache_get_reservable_items($content_types);
  }

  //get the array of data from the reservation base level
  $db_rows = cma_reservation_cache_get_rows_for_insert($cache_start_object,
						       $cache_end_object,
						       $content_types,
						       $items);

  //pass the data from the base level to the function that groups
  //inserts and delete statements and then executes them in the db
  cma_reservation_cache_remove_and_replace_db_rows($db_rows);
}