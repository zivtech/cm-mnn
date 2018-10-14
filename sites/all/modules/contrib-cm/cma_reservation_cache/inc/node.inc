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
    cma_reservation_cache_update_item_cache($node,$content_types[$node->type],
					    $op);
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