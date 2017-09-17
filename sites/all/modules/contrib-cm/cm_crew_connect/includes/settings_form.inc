<?php

/**
 * Setting Form for the Crew Connect Module
 */
function cm_crew_connect_admin_form($form) {

  $form['cm_crew_connect_event_template_id'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('CiviCRM Event Template ID'),
	  '#default_value' =>
	  variable_get('cm_crew_connect_event_template_id', 5360),
	  '#attributes' => array(
				 ' type' => 'number', 
				 ),
	  '#description' =>
	  t('Enter the ID of the CiviCRM Event Template'),
	  );

  $form['cm_crew_connect_event_type_id'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('CiviCRM Event Type ID'),
	  '#default_value' =>
	  variable_get('cm_crew_connect_event_type_id', 170),
	  '#attributes' => array(
				 ' type' => 'number', 
				 ),
	  '#description' =>
	  t('Enter the ID of the CiviCRM Event Type'),
	  );

  return system_settings_form($form);
}

