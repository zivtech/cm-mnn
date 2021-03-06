<?php

/**
 * Function will build the form and the chart of reservable items and their
 * resesrvations. This function is called from menu item "reservations/chart"
 */
function cma_reservation_cache_chart_form($form, $form_state) {
  cma_reservation_cache_log_time('-----------------------beginning of ' .
				     'cma_reservation_cache_chart_form');

  $form = array();
  $values = cma_reservation_cache_chart_filter_values();
  $default_values = cma_reservation_chart_default_filter_values($values);

  //ADD ALL THE FORM ELEMENTS FOR THE FILTERS, $form IS PASSED BY REFERENCE
  cma_reservation_cache_chart_add_filters_to_form($form, $values);

  //FORMS ARE OFTEN BUILT JUST FOR VALIDATION AND SUBMIT AND NOT FOR DISPLAY
  //SO WE WILL USE THIS BOOLEAN TO DECIDE WHETHER TO EXECUTE THE LOGIC FOR THE
  //PURE MARKUP ON THE PAGE, WHICH IS MOSTLY THE CHART ITSELF
  //WE SAVE A LOT OF PROCESSING TIME IN NOT DOING THE MARKUP
  $is_form_being_build_for_display = TRUE;

  //FIXME IS THERE A BETTER WAY TO DECIDE IF THIS IS THE FORM BID WE DON'T
  //WANT TO DO MARKUP FOR?
  if(!empty($form_state['input'])) {
    $is_form_being_build_for_display = FALSE;
  }

  //ONLY BUILD THE CHART IF THE FORM IS GOING TO BE DISPLAYED NOW RATHER
  //THAN REBUILT AFTER THE SUBMIT FUNCTIONALITY IS EXECUTED
  if ($is_form_being_build_for_display) {
    //GET ALL THE ITEM TIME SLOTS FROM CACHE TABLE BASED ON FILTER VALUES
    $item_time_slots =
      cma_reservation_cache_get_item_time_slots($default_values);
    
    //GET THE ACTUAL HTML FOR THE CHART
    $chart_display = cma_reservation_cache_get_chart_display($item_time_slots,
							     $default_values);

    //ADD THE HTML TO THE FORM AS MARKUP
    $form['chart'] = array('#markup'=> $chart_display);
  }
  cma_reservation_cache_log_time('Finished Processing Form and Chart');
  
  return $form;
}


/**
 * FUNCTION WILL VALIDATE THE CHART FORM FOR THINGS LIKE INCORRECT DAY/TIME
 */
function cma_reservation_cache_chart_form_validate($form, &$form_state) {
  $filters = cma_reservation_cache_filters();
  foreach ($filters as $filter) {
    //TODO FIXME ADD VALIDATION!
    //1. start day after last day
    //2. start time after last time
    //dsm($form_state['values'][$filter], 'my value for ' . $filter);
    //form_set_error($filter, t('error!'));
  }
  cma_reservation_cache_log_time('done with form_validate()');
}
/**
 */
function cma_reservation_cache_chart_form_submit($form, &$form_state) {
  $url = current_path();
  $args = array();

  //GET ARRAY OF AVAILABLE FILTERS AND ITERATE OVER THEM TO BUILD QUERY STRING
  $filters = cma_reservation_cache_filters();
  foreach ($filters as $filter) {
    if ($form_state['values']['op'] == 'Apply Filters') {
      $value = $form_state['values'][$filter];
      //IF THE VALUE IS AN ARRAY, TURN IT INTO A COMMA DELIMINATED STRING
      if (is_array($value)) {
	$value = implode(',', $value);
      }
      //IF WE HAVE A NON EMPTY VALUE, ADD IT TO THE ARRAY FOR QUERY STRING
      //KEY VALUE PAIRS
      if (!empty($value)) {
	$args[$filter] = $value;
      }
    }
    else if ($form_state['values']['op'] == 'Clear Filters') {
      //don't add anything to $args, since we are clearing filters
    }
  
  }
  cma_reservation_cache_log_time('done with form_submit()');

  //forward to the original url with a new set of query args
  drupal_goto($url,  array("query" => $args));
}
/**
 * Function is passed in a form array by reference and then adds all the 
 * form elements and sets default values based on the passed in values
 */
function cma_reservation_cache_chart_add_filters_to_form(&$form, $values) {

  $defaults = cma_reservation_chart_default_filter_values($values);

  //parent fields set
  $form['chart_filters'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Reservation Chart Filters"),
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE,
	  );

  ////////////////////////////////////////////////////////////////////////
  //day and time fields
  $form['chart_filters']['day_and_time'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Day and Time Filters"),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE,	  
	  );

  $form['chart_filters']['day_and_time'][CMA_RESERVATION_CACHE_FIRST_DAY] =
    array(
	  '#title' => t('First Day'),
	  '#date_format' => 'Y-m-d',
	  '#type' => 'date_popup',
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_FIRST_DAY],
	  '#description' => '',
	  );
  
  $form['chart_filters']['day_and_time'][CMA_RESERVATION_CACHE_LAST_DAY] =
    array(
	  '#title' => t('Last Day'),
	  '#date_format' => 'Y-m-d',
	  '#type' => 'date_popup',
	  '#date_label_position' => 'within',
	  '#size' => 15,
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_LAST_DAY],
	  '#description' => '',
	  );
  
  $hours = cma_reservation_cache_hours_array();
  unset($hours[24]);
  $form['chart_filters']['day_and_time'][CMA_RESERVATION_CACHE_BEGIN_HOUR] =
    array(
	  '#title' => t('Begin Hour'),
	  '#type' => 'select',
	  '#options' => $hours,
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_BEGIN_HOUR],
	  '#description' => '',
	  );

  $hours = cma_reservation_cache_hours_array();
  unset($hours[0]);
  
  $form['chart_filters']['day_and_time'][CMA_RESERVATION_CACHE_END_HOUR] =
    array(
	  '#title' => t('End Hour'),
	  '#type' => 'select',
	  '#options' => $hours,
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_END_HOUR],
	  '#description' => '',
	  );

  //ALLOWS USER TO FILTER BY DAY OF WEEK
  $form['chart_filters']['day_and_time'][CMA_RESERVATION_CACHE_DOW] =
    array(
	  '#title' => t("Day of Week"),
	  '#type' => 'select',
	  '#options' => array(0=>t('<-Select Day of Week->'),
			      1=>'Sunday',
			      2=>'Monday',
			      3=>'Tuesday',
			      4=>'Wednesday',
			      5=>'Thursday',
			      6=>'Friday',
			      7=>'Saturday',
			      ),
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_DOW],
	  );


  ////////////////////////////////////////////////////////////////////////
  //multiselect fields
  
  //build an array of the multislect fields to loop over
  $multiselect_fields =
    array('inventories' =>
	  array('filter' => CMA_RESERVATION_CACHE_INVENTORY,
		'fieldset_title' => t("Inventory Filter"),
		),
	  'categories' =>
	  array('filter' => CMA_RESERVATION_CACHE_BUCKET_CATEGORY,
		'fieldset_title' => t("Category Filter"),
		),
	  'content_types' =>
	  array('filter' => CMA_RESERVATION_CACHE_ITEM_TYPE,
		'fieldset_title' => t("Type Filter"),
		),
	  'resources' =>
	  array('filter' => CMA_RESERVATION_CACHE_ITEM_NID,
		'fieldset_title' => t("Resource Filter"))
	  );

  //these arrays will used for the default values in the multiselct fields
  //within the loop, but also used to filter themselves, i.e. the
  //inventories selected in the multiselect will affect which categories
  //will appear, and the selected categories will affect which content types
  //appear, and content types will affect which affect which resources to
  //appear
  $default_inventories = $default_categories = $default_content_types=array();

  //loop over the multiselect fields and add a fields set and the
  //multiselect widget
  foreach($multiselect_fields as $field_name => $field_info) {
    //call function to get options for the multiselect
    $options =  
      cma_reservation_cache_get_available_options($field_name,
						  $default_inventories,
						  $default_categories,
						  $default_content_types);
    if (empty($options)) {
      continue;
    }
    //add the fieldset
    $form['chart_filters'][$field_name] =
      array(
	    '#type' => 'fieldset',
	    '#title' => $field_info['fieldset_title'],
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
	    );
 
    //add the multiselect form element
    $form['chart_filters'][$field_name][$field_info['filter']] =
      array(
	    '#type' => 'multiselect',
	    '#multiple' => TRUE,
	    '#options' => $options,
	    '#default_value' => $defaults[$field_info['filter']],
	    );
    
    //set the default arrays for subsequent fetching and filtering
    if ($field_name == 'inventories') {
      $default_inventories = $defaults[$field_info['filter']];
    }
    else if ($field_name == 'categories') {
      $default_categories = $defaults[$field_info['filter']];
    }
    else if ($field_name == 'content_types') {
      $default_content_types = $defaults[$field_info['filter']];
    }
  } //end of the loop for multiselect fields

  ////////////////////////////////////////////////////////////////////////
  //staff 
  $form['chart_filters']['staff'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Staff Filters"),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE,	  
	  );
  $form['chart_filters']['staff'][CMA_RESERVATION_CACHE_MODE] =
    array(
	  '#title' => t("Mode"),
	  '#type' => 'select',
	  '#options' => array(0=>t("Public"),
			      1=>t("Staff"),
			      ),
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_MODE],
	  );

  $form['chart_filters']['staff'][CMA_RESERVATION_CACHE_SEPARATE_CHARTS] =
    array(
	  '#title' => t("One Combined Chart or Separate Chart for Each Day"),
	  '#type' => 'select',
	  '#options' => array(
			      0=>t("Combined"),
			      1=>t("Separate"),
			      ),
	  '#default_value' => $defaults[CMA_RESERVATION_CACHE_SEPARATE_CHARTS],
	  );

  $form['chart_filters']['submit'] = array(
					   '#type' => 'submit',
					   '#value' => t('Apply Filters'),
					   );
  $form['chart_filters']['clear'] = array(
					  '#type' => 'submit',
					  '#value' => t('Clear Filters'),
					  );
  cma_reservation_cache_log_time("Finished building filters");
    
}

/**
 * FUNCTION WILL RETURN AN ARRAY OF DEFAULT VALUES FOR CHART FILTER FORM 
 * ELEMENTS
 */
function cma_reservation_chart_default_filter_values($values) {
  $defaults = array();

  foreach($values as $key=>$value) {
    if (empty($value)) {
      if ($key == CMA_RESERVATION_CACHE_FIRST_DAY) {
	$value = date('Y-m-d');
      }
      else if ($key == CMA_RESERVATION_CACHE_LAST_DAY) {
	$value = date('Y-m-d');
      }
      else if ($key == CMA_RESERVATION_CACHE_BEGIN_HOUR) {
	$value = 0;
      }
      else if ($key == CMA_RESERVATION_CACHE_END_HOUR) {
	$value = 24;
      }
      else if ($key == CMA_RESERVATION_CACHE_INVENTORY) {
	$inventories = cma_reservation_cache_get_available_inventories();
	foreach ($inventories as $k=>$v) {
	  $value = array($k);
	  break;
	}
      }
      else if ($key == CMA_RESERVATION_CACHE_BUCKET_CATEGORY) {
	/**
	$default_inventories = $defaults[CMA_RESERVATION_CACHE_INVENTORY];
	$categories =
	  cma_reservation_cache_get_available_categories($default_inventories);
							 
	foreach ($categories as $k=>$v) {
	  $value = array($k);
	  break;
	}
	*/
	$value = array();
      }
      else if ($key == CMA_RESERVATION_CACHE_ITEM_TYPE) {
	$value = array();
      }
      else if ($key == CMA_RESERVATION_CACHE_ITEM_NID) {
	$value = array();
      }
      
    }
    $defaults[$key] = $value;
  }

  //dsm($defaults, 'defaults for form elements');
  //FIXME ADD HOOK TO ALLOW DEFAULTS TO BE OVERRIDDEN
  return $defaults;
}
/**
 * Function will return an array of filter values for the chart
 */
function cma_reservation_cache_chart_filter_values() {
  $values = array();
  $filters = cma_reservation_cache_filters();
  $multiple_value_filters =
    array(
	  CMA_RESERVATION_CACHE_INVENTORY,
	  CMA_RESERVATION_CACHE_BUCKET_CATEGORY,
	  CMA_RESERVATION_CACHE_ITEM_TYPE,
	  CMA_RESERVATION_CACHE_ITEM_NID,
	  );

  foreach ($filters as $filter) {
    //FIXME - CHECK CONFIG SETTINGS AND PERMISSIONS TO PREVENT USERS FROM
    //SETTINGS FILTERS THEY DON'T HAVE PERMISSION TO MESS WITH
    $value = filter_input(INPUT_GET, $filter);
    if (!empty($value) && in_array($filter, $multiple_value_filters)) {
	$values[$filter] = explode(',', filter_input(INPUT_GET, $filter));
    }
    else {
      $values[$filter] = $value;
    }
  }
  //dsm($values, 'submitted query string form values');
  return $values;
}

/**
 * Function is passed an array of item time slots and builds the html to 
 * display the actual chart (just the actual chart, not filter form elements
 * or any other markup 
 */
function cma_reservation_cache_get_chart_display($item_time_slots,
						 $filter_values) {
  $separate_charts =
    !empty($filter_values[CMA_RESERVATION_CACHE_SEPARATE_CHARTS]);

  //initialize the header row, the subsequent header cells are added during
  //the first loop of item time slots
  
  //iterate over all the item time slots, adding a new row when we change the
  //item (which will have its own unique $identifier).
  //it will also
  $counter =  $res_nid = $prev_res_nid = $prev_slot = 0;
  $reservation_slots = $identifiers = array();
  $chart_display = $charts_html = $chart_html = $row_html = $rows_html =
    $header_row_html = $identifier = $prev_identifier = $first_day = '';
  $has_slots = FALSE;
  foreach ($item_time_slots as $slot) {
    $has_slots = TRUE;
    $slot_needs_display = FALSE;
    $counter ++;
    $day = date('Y-m-d',$slot->cache_slot_start_time);
    if (empty($first_day)) {
      $first_day = $day;
    }
    
    $identifier = cma_reservation_cache_slot_identifier($slot);    
    $identifiers[] = $identifier;
    $res_nid = $slot->reservation_nid;

    //if we have no html at all yet, we are starting first row of first chart
    if (empty($charts_html) && empty($chart_html)) {
      //dsm($slot, 'slot 1');
      //begin teh chart and row
      $chart_html = "<div class='cma_reservation_cache_chart'>" .
	cma_reservation_cache_chart_title_html($slot, $filter_values);

      $chart_html .= '<div class="cma_reservation_cache_chart_table">';
      
      $row_html = '<div class="cma_reservation_cache_chart_row">';
      $row_html .= '<div class="cma_reservation_cache_chart_first_cell">' .
	cma_reservation_cache_slot_item_title($slot) . "</div>";
      $header_row_html =
	'<div class="cma_reservation_cache_chart_header_row">';
      $header_row_html .=
       '<div class="cma_reservation_cache_chart_first_header_cell">Item</div>';
    }
    //we are doing a different chart per day, so because its a new day,
    //end previous row and chart and open new row and chart
    else if (!empty($separate_charts) && !empty($prev_day) &&
	     $day != $prev_day) {
      //dsm($slot, 'slot 2');
      if (!empty($reservation_slots)) {
	$row_html .=
	  cma_reservation_cache_reservation_slots_html($reservation_slots);
	$reservation_slots = array();
      }
      //add the last row to the html for all the row divs
      $rows_html .= $row_html . "</div>";
      //add the all the rows and closing div to the last chart
      $chart_html .= $header_row_html . "</div>" . $rows_html . "</div>";
      //add the last chart to the html for all the charts
      $charts_html .= $chart_html . "</div>";

      //reset for new chart and row
      $chart_html = "<div class='cma_reservation_cache_chart'>" .
	cma_reservation_cache_chart_title_html($slot, $filter_values);

      $chart_html .= '<div class="cma_reservation_cache_chart_table">';
      $rows_html = '';	    
      $row_html = '<div class="cma_reservation_cache_chart_row">';
      $row_html .= '<div class="cma_reservation_cache_chart_first_cell">' .
	cma_reservation_cache_slot_item_title($slot) . "</div>";
      $header_row_html =
	'<div class="cma_reservation_cache_chart_header_row">';
      $header_row_html .=
       '<div class="cma_reservation_cache_chart_first_header_cell">Item</div>';

    }
    //our identifier has changed so end this row and begin a new one 
    else if ($prev_identifier != $identifier) {
      /*
      dsm($prev_slot, 'prev slot'.
	  date('Y-m-d H:i', $prev_slot->cache_slot_start_time) .
	  " " . $prev_slot->item_title);
      
      dsm($slot, 'slot 3'.date('Y-m-d H:i', $slot->cache_slot_start_time) .
	  " " . $slot->item_title);
      */
      //since row is ending, close the reservation slot block
      if (!empty($reservation_slots)) {
	$row_html .=
	  cma_reservation_cache_reservation_slots_html($reservation_slots);
	$reservation_slots = array();
      }
      //add the last row to the html for all the row divs
      $rows_html .= $row_html . "</div>";

      //reset for new row
      $row_html = '<div class="cma_reservation_cache_chart_row">';
      $row_html .= '<div class="cma_reservation_cache_chart_first_cell">' .
	cma_reservation_cache_slot_item_title($slot) . "</div>";
      
      //if rows is empty this is the first row of this chart so add header
      if (empty($rows_html)) {
	$header_row_html =
	  '<div class="cma_reservation_cache_chart_header_row">';
	$header_row_html .=
	  '<div class="cma_reservation_cache_chart_first_header_cell">'.
	  'Item</div>';
      }
    
    }
    //done with checking for row end

    //look to see if a reservation block started and if it needs to be
    //added to or ended and renewed
    if (!empty($prev_res_nid) && empty($res_nid)) {
      $row_html .=
	cma_reservation_cache_reservation_slots_html($reservation_slots);
      $reservation_slots = array();
    }
    else if (!empty($prev_res_nid) && !empty($res_nid) &&
	     $prev_res_nid != $res_nid) {
      $row_html .=
	cma_reservation_cache_reservation_slots_html($reservation_slots);
      $reservation_slots = array($slot);
    }
    else if (!empty($prev_res_nid) && !empty($res_nid)) {
      $reservation_slots[] = $slot;
    }
    else if (empty($prev_res_nid) && !empty($res_nid)) {
      $reservation_slots = array($slot);
    }
    else {// if (empty($prev_res_nid && empty($res_nid))) {
      $reservation_slots = array();
    }

    if (empty($separate_charts) && $first_day != $day && !empty($prev_day) &&
	$day != $prev_day) {
      if (empty($rows_html)) {
	$header_row_html .=
	 '<div class="cma_reservation_cache_chart_header_cell" '.
	'style="background-color:black;"></div>';
      }
      if (!empty($reservation_slots)) {
	$temp_slot = array_pop($reservation_slots);
	$row_html .=
	  cma_reservation_cache_reservation_slots_html($reservation_slots);
	$reservation_slots = array($temp_slot);
      }
      $row_html .=
      '<div class="cma_reservation_cache_chart_cell" '.
      	'style="background-color:black;"></div>';	
    }
    //if $rows is empty, this will be first row of a chart, therefore add
    //header
    if (empty($rows_html)) {
      
      $header_row_html .=
	'<div class="cma_reservation_cache_chart_header_cell">'.
	cma_reservation_cache_slot_header_display($slot)."</div>";
    }
    //if this is empty it means we need to display it as a regular cell
    //rather than part of a reservation slot block
    if (empty($reservation_slots)) {
      //add regular chart slot, first calling functions with overridable hooks
      //to get the hover text and the display html for the actual cell in chart
      $hover = cma_reservation_cache_chart_slot_hover($slot);
      $slot_display = cma_reservation_cache_chart_slot_display($slot);
      $color = cma_reservation_cache_slot_background_color($slot);
      $row_html .= '<div style="background-color:'.$color.';" title="'.$hover.
	'" class="cma_reservation_cache_chart_cell">'.$slot_display . "</div>";
    }

    //set prev so subsequent loops can see if item/day changes and therefore
    //know to start a new row div
    $prev_identifier = $identifier;
    $prev_res_nid = $res_nid;
    $prev_day = $day;
    $prev_slot = $slot;

  }//done looping over item time slots

  //if we have slots after the loop, add the block to the row html
  if (!empty($reservation_slots)) {
    $row_html .=
      cma_reservation_cache_reservation_slots_html($reservation_slots);
  }
  
  if ($has_slots) {
    //add the last row to the html for all the row divs
    $rows_html .= $row_html . "</div>";
    //add the all the rows and closing div to the last chart
    $chart_html .= $header_row_html . "</div>" . $rows_html . "</div>";
    //add the last chart to the html for all the charts
    $charts_html .= $chart_html . "</div>";
    
    //close the div on the last row div and the table div
    $chart_display .= '<div class"cma_reservation_cache_chart_display"'.
      $charts_html . "</div>";
  }
  else {
    drupal_set_message(t("No results found with the filters selected. " .
			 "Either change filters or reset them to display " .
			 "the chart."), 'error');
  }
  //LOGGING
  $msg =
    "finished building chart html with " . count(array_unique($identifiers)).
    " resource/bucket items and " . $counter ." item time slots";
  cma_reservation_cache_log_time($msg);

  return $chart_display;
}

/**
 * Function returns a title with filter information for the chart 
 */
function cma_reservation_cache_chart_title_html($slot,
						$filter_values) {

  $ret = "<div class='cma_reservation_cache_chart_title'>" .
    "<h2>Reservable Item Chart for ";

  $start = $filter_values[CMA_RESERVATION_CACHE_FIRST_DAY];
  $end = $filter_values[CMA_RESERVATION_CACHE_LAST_DAY];
  
  if ($start != $end &&
      empty($filter_values[CMA_RESERVATION_CACHE_SEPARATE_CHARTS])) {
    $start_ts = strtotime($start);
    $end_ts = strtotime($end);
    $ret .= date('l, F j, Y', $start_ts)." through ".date('l, F j, Y',$end_ts);
  }
  else {
    $ret .= date('l, F j, Y', $slot->cache_slot_start_time);
  }
  $ret .= "</h2>";
  
  $filter_string = '';
  $first_time = TRUE;
  foreach($filter_values as $key=>$value) {
    if ($key == CMA_RESERVATION_CACHE_INVENTORY) {
      $name = t("Inventory");
      $id_type = "tid";
    }
    else if ($key == CMA_RESERVATION_CACHE_BUCKET_CATEGORY) {
      $name = t("Category");
      $id_type = "tid";
    }
    else if ($key == CMA_RESERVATION_CACHE_ITEM_TYPE) {
      $name = t("Type");
      $id_type = "name";
    }
    else if ($key == CMA_RESERVATION_CACHE_ITEM_NID) {
         $name = t("Resource");
      $id_type = "nid";
    }
    else {
      continue;
    }

    if (!empty($value)) {
      if ($first_time) {
	$first_time = FALSE;
      }
      else {
	$filter_string .= "<br/>"; 
      }
      
      $filter_string .= $name . ": ";
      $first = TRUE;
      $filter_value_name = '';
      foreach($value as $id) {
	if ($id_type == 'tid') {
	  $object = taxonomy_term_load($id);
	  $filter_value_name = $object->name;
	}
	else if ($id_type == 'nid') {
	  $object = node_load($id);
	  $filter_value_name = $object->title;
	}
	else if ($id_type == 'name') {
	  $types = node_type_get_types();
	  $object = $types[$id];
	  $filter_value_name = $object->name;
	}
	if ($first) {
	  $first = FALSE;
	}
	else {
	  $filter_string .= ", "; 
	}
	
	$filter_string .= $filter_value_name; 
      }
    }
  }
  if (!empty($filter_string)) {
    $ret .= "<h4>" . $filter_string . "</h4>";
  }
  $ret .= "</div>";

  //FIXME ADD HOOK SO TITLE CAN BE OVERRIDDEN
  return $ret;
}
/**
 * Function returns the divs for a reservation to display on chart
 */
function cma_reservation_cache_reservation_slots_html($slots) {
  
  $html = "";

  $count = count($slots);

  if ($count > 0) {
    $center = ceil($count/2);
  }
  
  $index = 0;
  foreach ($slots as $slot) {
    $classes = "cma_reservation_cache_chart_cell";
    $hover = cma_reservation_cache_chart_slot_hover($slot);

    $index ++;

    if ($index == 1 && $index == $center && $index == $count) {
      //reservation block is a total of one cell long, treat it like normal
      $slot_display = cma_reservation_cache_chart_slot_display($slot);
    }
    else if ($index == 1 && $index == $center) {
      $classes .= " cma_reservation_cache_chart_res_block_start_cell";
      $classes .= " cma_reservation_cache_chart_res_block_title_cell";
      $slot_display = cma_reservation_cache_chart_slot_display($slot);
    }
    else if ($index == 1) {
      $classes .= " cma_reservation_cache_chart_res_block_start_cell";
      $slot_display = cma_reservation_cache_chart_slot_display($slot, TRUE);
    }    
    else if ($index == $center) {
      //add block center div
      $classes .= " cma_reservation_cache_chart_res_block_middle_cell";
      $classes .= " cma_reservation_cache_chart_res_block_title_cell";
      $slot_display = cma_reservation_cache_chart_slot_display($slot);
    }
    else if ($index < $count){
      //add block middle div
      $classes .= " cma_reservation_cache_chart_res_block_middle_cell";
      $slot_display = cma_reservation_cache_chart_slot_display($slot, TRUE);
    }
    else { //$index == $count
      //add block end div
      $classes .= " cma_reservation_cache_chart_res_block_end_cell";
      $slot_display = cma_reservation_cache_chart_slot_display($slot, TRUE);
    }

    $color = cma_reservation_cache_slot_background_color($slot);    
    $html .= '<div style="background-color:'.$color.';" title="' . $hover .
      '" class="' . $classes . '">'.$slot_display . "</div>";
  }

  return $html;
}
/**
 * Function will return the display to go into the div hover for a chart cell
 */
function cma_reservation_cache_chart_slot_hover($slot) {
  $fields = cma_reservation_cache_db_table_columns();
  $offset = cma_reservation_cache_get_offset();
  $start = date('Y-m-d h:ip',
		strtotime($slot->field_reservations_date_start) + $offset);
  $end = date('Y-m-d h:ip',
		strtotime($slot->field_reservations_date_end) + $offset);
  
  if ($slot->reservation_nid) {
    $ret = $slot->project_title;
    $ret .= "\n";
    $ret .= $slot->reservation_user_name;
    $ret .= "\n";
    $ret .= $start;
    $ret .= "\n";
    $ret .= $end;
  }
  else {
    $ret .= cma_reservation_cache_slot_item_title($slot);
    $ret .= "\n";
    $ret .= 'Start = ' . date('Y-m-d h:ip', $slot->cache_slot_start_time);
    $ret .= "\n";
    $ret .= 'End = ' . date('Y-m-d h:ip', $slot->cache_slot_end_time);
    $ret .= "\n";
  }

  /**
  foreach($fields as $name => $field) {
    $ret .= $name . ' = ' . strip_tags($slot->{$name}) . "\n";
  }
  */
  
  //FIXME ADD A HOOK SO THE HOVER CAN BE OVERRIDEN
  return $ret;
}
/**
 * Function returns display for a chart header cell
 */
function cma_reservation_cache_slot_header_display($slot) {
  $ret = date('n/j/y', $slot->cache_slot_start_time) . "<br/>" .
    date('g:ia', $slot->cache_slot_start_time);

  return $ret;
}

/**
 * Function will return the correct link for a chart slot
 */
function cma_reservation_cache_chart_slot_display($slot, $blank = FALSE) {
  if ($blank && $slot->reservation_nid) {
    $text = "&nbsp;<br/>&nbsp";
    $ret = l($text, "node/".$slot->reservation_nid,
	     array('html' => TRUE)); 
  }
  else {
    if (!empty($slot->reservation_nid)) {
      $text .= $slot->reservation_nid. "<br/>" .$slot->reservation_status_name;
      $ret .= l($text, "node/" . $slot->reservation_nid,
		array('html' => TRUE)); 
    }
    else {
      $text = date('h:ia', $slot->cache_slot_start_time) . "<br/>";

      $text .= '+' . "<br/>";
      $ret .= l($text, "node/add/reservations-reservation",
		array('html' => TRUE)); 
    }
  }
  //FIXME: ADD A HOOK HERE SO CAN OVERRIDE SLOT DISPLAY TEXT
  return $ret;
}
