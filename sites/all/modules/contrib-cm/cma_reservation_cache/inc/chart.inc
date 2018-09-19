<?php

/**
 * Function will build the form and the chart of reservable items and their
 * resesrvations. This function is called from menu item "reservations/chart"
 */
function cma_reservation_cache_chart_form() {
  dsm(date('h:i:s'), 'form start');
  cma_reservation_cache_log_microsec('form start'); 

  $form = array();
  $values = cma_reservation_cache_chart_filter_values();

  //ADD ALL THE FORM ELEMENTS FOR THE FILTERS, $form IS PASSED BY REFERENCE
  cma_reservation_cache_chart_add_filters_to_form($form, $values);

  //FIXME only here for testing
  return $form;
  
  //GET ALL THE ITEM TIME SLOTS FROM CACHE TABLE BASED ON FILTER VALUES
  $item_time_slots = cma_reservation_cache_get_item_time_slots($values);

  //GET THE ACTUAL HTML FOR THE CHART
  $chart_html = cma_reservation_cache_get_chart_html($item_time_slots);  

  //ADD THE HTML TO THE FORM AS MARKUP
  $form['chart'] = array('#markup'=> $chart_html);
  
  cma_reservation_cache_log_microsec('end of cma_reservation_cache_chart');
  dsm(date('h:i:s'), 'form end');
  
  return $form;
}


/**
 * Function is passed in a form array by reference and then adds all the 
 * form elements and sets default values based on the passed in values
 */
function cma_reservation_cache_chart_add_filters_to_form(&$form, $values) {

  $form['chart_filters'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Project Field Settings"),
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE,
	  );

   $form['chart_filters']['first_day'] =
     array(
	   '#title' => t('Start Day'),
	   '#date_format' => 'Y-m-d',
	   '#type' => 'date_popup',
	   '#date_label_position' => 'within',
	   '#size' => 15,
	   '#default_value' => $values[CMA_RESERVATION_CACHE_FIRST_DAY],
	   '#description' => '',
	   );
   
}
/**
 * Function is passed an array of item time slots and builds the html to 
 * display the actual chart (just the actual chart, not filter form elements
 * or any other markup 
 */
function cma_reservation_cache_get_chart_html($item_time_slots) {
  cma_reservation_cache_log_microsec('starting to build chart html');

  //adding the start of the table div
  $chart_html = '<div class="cma_reservation_cache_chart_table">';

  //get the header row div
  //it will use the times in the first item of the slots array
  $header = cma_reservation_cache_chart_header_row($item_time_slots);
  
  //identifer will be the unique item identifier, whether its a bucket or 
  //resource
  $identifier = $prev_identifier = '';

  //add all the data rows
  foreach ($item_time_slots as $slot) {
    $identifier = cma_reservation_cache_slot_identifier($slot);

    //if true, this is either first time through or we are starting a new row
    if ($prev_identifier != $identifier) {
      //first time through, add header row before we actually add the first row
      if (empty($prev_identifier)) {
	$chart_html .= $header;
      }
      //not the first time through so we close the div on the previous row
      else {
	$chart_html .= "</div>";
      }
      //start a new div row div
      $chart_html .= '<div class="cma_reservation_cache_chart_row">';

      //add a title to the first cell in the row using a function that
      //contains an overridable hook
      $chart_html .= '<div class="cma_reservation_cache_chart_first_cell">' .
	cma_reservation_cache_slot_item_title($slot) . "</div>";
    }
    //set prev so subsequent loops can see if item/day changes and therefore
    //know to start a new row div
    $prev_identifier = $identifier;

    //add regular chart slot, first calling functions with overridable hooks
    //to get the hover text and the display html for the actual cell in chart
    $hover = cma_reservation_cache_chart_slot_hover($slot);
    $slot_display = cma_reservation_cache_chart_slot_display($slot);
    $chart_html .= '<div title="' . $hover .
      '" class="cma_reservation_cache_chart_cell">' . $slot_display . "</div>";
  }
  //done with loop, close the div on the last row div and the table div
  $chart_html .= "</div></div>";

  cma_reservation_cache_log_microsec('finished building chart html');
  return $chart_html;
}

/**
 * Function will return a header row by iterating over item_time_slots and
 * finding the first row
 */
function cma_reservation_cache_chart_slot_hover($slot) {
  $fields = cma_reservation_cache_db_table_columns();
  $ret = 'Start = ' . date('h:ip', $slot->cache_slot_start_time);
  $ret .= "\n";
  $ret .= 'End = ' . date('h:ip', $slot->cache_slot_end_time);
  $ret .= "\n";
  
  foreach($fields as $name => $field) {
    $ret .= $name . ' = ' . strip_tags($slot->{$name}) . "\n";
  }

  //FIXME ADD A HOOK SO THE HOVER CAN BE OVERRIDEN
  return $ret;
}

/**
 * Function will return the correct link for a chart slot
 */
function cma_reservation_cache_chart_slot_display($slot) {
  $ret = date('h:ip', $slot->cache_slot_start_time) . "<br/>";
  if (!empty($slot->reservation_nid)) {
    $ret .= l($slot->reservation_nid, "node/" . $slot->reservation_nid); 
  }
  else {
    $ret .= l('+', "node/add/reservations_reservation");
  }

  //FIXME: ADD A HOOK HERE SO CAN OVERRIDE SLOT DISPLAY TEXT
  return $ret;
}
/**
 * Function will return a header row by iterating over item_time_slots and
 * finding the first row
 */
function cma_reservation_cache_chart_header_row($item_time_slots) {
  $header = '<div class="cma_reservation_cache_chart_header_row">';
  $header .=
    '<div class="cma_reservation_cache_chart_first_header_cell">Item</div>';

  $identifer = $prev_identifier = '';
  
  //BUILD THE HEADER ROW
  foreach ($item_time_slots as $slot) {
    //get unique identifer for row
    $identifier = cma_reservation_cache_slot_identifier($slot);

    //check to see if we are still on first row of results
    if (!empty($prev_identifier) && ($prev_identifier != $identifier)) {
      break;
    }
    //add the header cell
    $header .= '<div class="cma_reservation_cache_chart_header_cell">' .
      date('m-d', $slot->cache_slot_start_time) . "<br/>" .
      date('H:i', $slot->cache_slot_start_time) . "</div>";
    
    $prev_identifier = $identifier;
  }
  //close the header row
  $header .= "</div>";
  return $header;
}