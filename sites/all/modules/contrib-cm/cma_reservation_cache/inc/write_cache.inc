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