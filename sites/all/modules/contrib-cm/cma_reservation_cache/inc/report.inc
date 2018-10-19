<?php


/**
 * Form gives information on cache and tools to reload it
 * @param type $form
 * @return $form
 */
function cma_reservation_cache_report($form) {
  $form = array();

  $min = cma_reservation_cache_get_min_cache_date();
  $max = cma_reservation_cache_get_max_cache_date();

  $msg = "<h2>Report</h2>";
  $msg .= "From " . date('Y-m-d H:i', $min) . " to " . date('Y-m-d H:i', $max);
  $form['cache']['header'] = array('#markup' => $msg ."<br/><br/>");
								     
  //number of content types
  //number of bucket slots
  //number of resource items
  //earliest date
  //latest date
  $form['cache']['start_date'] =
    array(
	  '#title' => 'Start of Cache Refresh',
	  '#type' => 'date_popup',
	  '#date_format' => "Y-m-d",
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => date("Y-m-d"),
	  '#description' => '',
	  );
  $form['cache']['end_date'] =
    array(
	  '#title' => 'End of Cache Refresh',
	  '#type' => 'date_popup',
	  '#date_format' => "Y-m-d",
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => date("Y-m-d"),
	  '#description' => '',
	  );

  $form['submit'] =
    array('#type' => 'submit',
	  '#value' => t('Clear and Reload Cache'),
	  );
  return $form;
}

/**
 *
 */
function cma_reservation_cache_report_submit($form, $form_state) {
  //get start and end from the submitted form values
  $cache_start = $form_state['values']['start_date'] . " 00:00:00";
  $cache_end = $form_state['values']['end_date'] . " 23:59:59";
  $cache_start_object = new DateTime($cache_start);
  $cache_end_object = new DateTime($cache_end);

  $content_types = reservations_content_types();

  $items = cma_reservation_cache_get_reservable_items();
  
  //get the array of data from the reservation base level
  $db_rows = cma_reservation_cache_get_rows_for_insert($cache_start_object,
						       $cache_end_object,
						       $content_types,
						       $items);

  //pass the data from the base level to the function that groups
  //inserts and delete statements and then executes them in the db
  cma_reservation_cache_insert_and_update_db_rows($db_rows);
}
