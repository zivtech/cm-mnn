<?php


/**
 * Loads the general settings form.
 *
 * @see cma_reservation_cache_admin_form()
 */
function cma_reservation_cache_settings_home() {
  return drupal_get_form('cma_reservation_cache_settings_form');
}

/**
 * @param type $form
 * @return $form
 */
function cma_reservation_cache_settings_form($form) {
  $form = array();

  $form['project_fieldset'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Project Field Settings"),
	  );

  $fields_info = field_info_instances('node', 'reservations_reservation');
  $project_fields = array(0=>'<Select>');
  foreach ($fields_info as $field_name => $value) {
    $project_fields[$field_name] = $field_name;
  }
  asort($project_fields);

  $default_value = variable_get('cma_reservation_cache_project_field', '');
  $form['project_fieldset']['cma_reservation_cache_project_field']
    = array(
	    '#type' => 'select',
	    '#title' =>t('Which field on reservations is the project?'),
	    '#required' => FALSE,
	    '#options' => $project_fields,
	    '#default_value' => $default_value,
	    );


  //////////////////////////////////////////////////////////////////////////
  $rows = array();
  $row_elements = array();
  $data=array();


  //BUCKET CATEGORIES  
  $vid = variable_get('reservations_grouping_vid', 0);
  $terms = taxonomy_get_tree($vid);
  $bucket_categories = array();
  foreach ($terms as $term) {
    $bucket_categories[$term->tid] = $term->name; 
  }
  natcasesort($bucket_categories);

  $default_weight = -100;
  foreach ($bucket_categories as $k =>$v) {
    $default = variable_get($k.'_rank_weight',  $default_weight++);
    // Build the table rows.
    $rows[$k] =
      array(
	    'data' =>
	    array(
		  // Cell for the cross drag&drop element.
		  array('class' => array('item-cross')),
		  // Weight item for the tabledrag.
		  array('data' =>
			array(
			      '#type' => 'weight',
			      '#title' => t('Weight'),
			      '#title_display' => 'invisible',
			      '#default_value'=>$default, 
			      '#delta' => 500,
			      '#attributes' =>
			      array('class' => array('item-order-weight'),),
			      )),
		  //BUCKET CATEGORY NAME MARKUP
		  array('data' => array('#markup' => "$v",)),
		  
		  ),
	    'class' => array('draggable'),
	    );
    
    // Build rows of the form elements in the table.
    $row_elements[$k] =
      array(
	    $k.'_rank_weight' => &$rows[$k]['data'][1]['data'],
	    'bucket_category_'.$k => &$rows[$k]['data'][2]['data'],
	    );
  }

  //sort rows on the weight value
  uasort($rows, function($a, $b) {
      return $a['data'][1]['data']['#default_value'] >=
	$b['data'][1]['data']['#default_value'];
    });

  $form['cma_reservation_chart'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t("Chart Settings"),
	  );

  $form['cma_reservation_chart']['sort'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => t('Order of Categories on Chart'),
	  );

  // add this to parent form
  $form['cma_reservation_chart']['sort']['bucket_categories'] =
    array(
	  '#theme' => 'table',
	  // The row form elements need to be processed and build,
	  // therefore pass them as element children.
	  'elements' => $row_elements,
	  '#header' =>
	  // We need two empty columns for the weigth field and th10e cross.
	  array(
		array('data' => t('Sort'), 'colspan' => 2),
		array('data' => t('Reservable Content Type Category')),
		),
	  '#rows' => $rows,
	  '#attributes' => array('id' => 'item-order'),
	  '#sufix'=>'</div>',
	  );
  drupal_add_tabledrag('item-order', 'order', 'sibling','item-order-weight');
  
  return system_settings_form($form);
}
