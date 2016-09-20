<?php

/**
 * @file
 * Reservations Inventory
 */

 /**
 * Builds the admininstration settings form.
 */
function reservations_inventory_admin_settings($form, &$form_state) {

  $vid = variable_get('reservations_inventory_vid', 0);   
  $tid = variable_get('reservations_inventory_default_reservation_inventory', '');
      
   // weighted -9
   $grouping_array = reservations_taxonomy_term_select(t('Default Inventory'), NULL, $tid, $vid, t('If configured, when creating a reservation the reservable items will be filtered to this inventory by default.'), 0, '<' . t('Select') . '>');

  $form['reservations_inventory_default_reservation_inventory'] = $grouping_array;

  // Hours of Operation
  $hours_of_operation = reservations_inventory_load_hours_of_operation();
  //dpm($hours_of_operation, "hours of operation settings");
  
  $hours_description = '<div>' . t('Configure hours of operations for each specific inventory.') . '</div>';
  $inventory_hours_description = t('Enter military time for both opening and closing time, separated by a dash, in the format') . ' <em>hh:mm-hh:mm</em><br />' . t('ex.') . ' <em>09:00-17:00</em> ' . t('would be open at 9AM, close at 5PM. Leave blank to indicate not being open.');

  $hours_element = array(
    '#type' => 'textfield',
    '#title' => t('Monday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#element_validate' => array('reservations_inventory_admin_validate_hours_of_operation'),
    );

  $days_of_the_week = array(
    'sunday',
    'monday',
    'tuesday',
    'wednesday',
    'thursday',
    'friday',
    'saturday',
  );
  
  $form['reservations_hours_title'] = array(
    '#type' => 'item',
    '#title' => t('Hours of operation'),
    '#description' => $hours_description,
  );
      // vertical tab holder - contains all sub tabs
  $form['reservations_hours'] = array(
    '#type' => 'vertical_tabs',

  );
  
  $tree = taxonomy_get_tree($vid);

  foreach ($tree as $term) {
    // Adds Hours of Operations Settings for Each Active Inventory  

    $term_tid = $term->tid;
    
    $form['reservations_hours'][$term_tid] = array(
      '#type' => 'fieldset',
      '#title' => t($term->name . ' Inventory'),
      '#collapsible' => TRUE,
      '#group' => 'reservations_hours',
      '#weight' => $term->weight,      
    );
    
    $form['reservations_hours'][$term_tid]['description'] = array(
      '#type' => 'item',
      '#title' => t($term->name . ' Inventory'),
      '#description' => $inventory_hours_description,
      '#group' => 'reservations_hours',         
    );    
    
    foreach ($days_of_the_week as $num => $day) {
      $form['reservations_hours'][$term_tid]
            ['reservations_inventory_hours_' . $day . '_' . $term_tid] = array(
        '#title' => t('@day Hours', array('@day' => drupal_ucfirst($day))),
        '#default_value' => isset($hours_of_operation[$term_tid][$num]) ? 
              implode('-', $hours_of_operation[$term_tid][$num]) : '',
        '#type' => 'textfield',
        '#size' => 11,
        '#maxlength' => 11,
        '#element_validate' => array('reservations_inventory_admin_validate_hours_of_operation'),
        '#group' => 'reservations_hours',
      );
    }    
    
    $form['reservations_hours'][$term_tid]
             ['reservations_inventory_hours_admin'. '_' . $term_tid] = array(
      '#type' => 'textfield',
      '#title' => t('Expanded Hours'),
      '#default_value' => isset($hours_of_operation[$term_tid]['admin']) ?
             implode('-', $hours_of_operation[$term_tid]['admin']) : '',
      '#description' => 
        t(
          "This setting controls the hours shown on the conflict grid for " .
          "users with 'Create new content (Outside Normal Hours of " . 
          "Operation)' permission.  Normally, the conflict grid is limited " .
          "to the hours of operation set above for the days of the week. " .
          "But user allowed to create reservations outside normal hours of " .
          "operation, need to see conflicts during the expanded hours as well."
        ),
      '#size' => 11,
      '#maxlength' => 11,
      '#element_validate' => 
         array(
          'reservations_inventory_admin_validate_hours_of_operation'
         ),
      '#group' => 'reservations_hours',
    );
    
  }
  
  //dpm($form_state, "form_state settings");

  // Don't want to save each of the form variables separately in the table
  // so we handle the system_settings_form ourselves
  $form = reservations_inventory_system_settings_form($form);
  $form['#submit'][] = 'reservations_inventory_admin_settings_submit';
  
  //return system_settings_form($form);
  return ($form);
}

 /**
 * Builds an array representing the hours of operation for the facility
 * for producers and users who can create reservations outside normal hours.
 *
 *   A multidimensional arry with the following key/value pairs:
 *     [term_tid] => An associative array with the following key/value pairs:
 *        [day_of_week or admin] => An associative array with the 
 *          following key/values pairs:
 *             'open'  => Opening time (military).
 *             'close' => Closing time (military).
 */
function reservations_inventory_admin_settings_submit($form, &$form_state) {
  //dpm($form_state, "form_state submit"); 

  $vid = variable_get('reservations_inventory_vid', 0); 
  $tree = taxonomy_get_tree($vid);
  
  $days_of_the_week = array(
    'sunday',
    'monday',
    'tuesday',
    'wednesday',
    'thursday',
    'friday',
    'saturday',
  );
    
   $hours_of_operation = array();
    
  
  foreach ($tree as $term) {
    $term_tid = $term->tid;

    foreach ($days_of_the_week as $num => $day) {
      $hours = $form_state['values']
            ['reservations_inventory_hours_' . $day . '_' . $term_tid];
      if (empty($hours)) {
        $hours_of_operation[$term_tid][$num] = NULL;
      }
      else {
        list ($open, $close) = explode('-', $hours);
        $hours_of_operation[$term_tid][$num]['open'] = trim($open);
        $hours_of_operation[$term_tid][$num]['close'] = trim($close);
      }
    }  
     
    $expanded_hrs = $form_state['values']
            ['reservations_inventory_hours_admin_' . $term_tid];
    if (empty($expanded_hrs)) {
      $hours_of_operation[$term_tid]['admin'] = NULL;
    }
    else {
      list ($open, $close) = explode('-', $expanded_hrs);
      $hours_of_operation[$term_tid]['admin']['open'] = trim($open);
      $hours_of_operation[$term_tid]['admin']['close'] = trim($close);
    }

    // Need to determine if the hours of operation have been 
    // input for this tid
    $hours_tid = $hours_of_operation[$term_tid];
    foreach($hours_tid as $key => $value) {
      if(!is_null($hours_tid[$key]['open'])) {
        $exist_inventory_hrs = TRUE;
        break;
      }
      else {
        $exist_inventory_hrs = FALSE;
      }
    }
    $hours_of_operation[$term_tid]['exist_inventory_hrs'] = $exist_inventory_hrs;    

  }
  //dpm($hours_of_operation, 'hours submit');
  variable_set('reservations_inventory_hours_operation', $hours_of_operation);
  
}

/**
 * Helper function to validate closed hours or operation formatting.
 */
function reservations_inventory_admin_validate_hours_of_operation($element, &$form_state) {
  
  if (!empty($element['#value'])) {
    $time = array();
    $i = 0;
    $values = explode('-', $element['#value']);
    
    foreach ($values as $value) {      
      list ($hour, $min) = explode(':', $value);      
      
      if ($hour > 24) {  
        form_error($element, t('Hour must be less than or equal to 24.'));
      }
      if ($hour < 0) {
        form_error($element, t('Hour must be greater than or equal to 0.'));
      }
      if ($hour == 24 && $min > 0) {
        form_error($element, t(
            'If hour is equal to 24 then minutes must be equal to 0.'));
      }      
      if ($min > 59) {
        form_error($element, t('Minutes must be less than or equal to 59.'));
      }
      if ($min < 0) {
        form_error($element, t('Minutes must be greater than or equal to 0.'));
      }
      
      $time[$i]['hour'] = $hour;
      $time[$i]['min'] = $min;
      $i++;
    }
    
    // we need to catch values[1] < values[0] for circumstances where user
    // does not input values[1] as military time (such as 12:00-5:00)
    if ($time[1]['hour'] < $time[0]['hour']) {
form_error($element, t('Closing time must be greater then opening time.'));      
    }
    elseif ($time[1]['hour'] == $time[0]['hour'] && 
        $time[1]['min'] <= $time[0]['min']) {
form_error($element, t('Closing time must be greater then opening time.'));      
    }
  }  
}
