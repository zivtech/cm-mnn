<?php

/**
 * Function will set the project title suffix and all the field names for 
 * each kind of import
 */
function cma_program_import_get_variables($import_type) {
  $variables = array();
  $variables['type'] = $import_type;
  
  if ($import_type == 'fstv') {
    $variables['suffix']= ' [FSTV]';
    $variables['project'] = 'Program EPG Title';
    $variables['duration'] = 'Schedule Item Duration';
    $variables['date'] = 'Schedule Item Log Date';
    $variables['start'] = 'Schedule Item Log Time';
    $variables['time_type'] = 'h:m:s';
    $variables['show'] = 'Program EPG Title';
    $variables['show_subtitle'] = '';
    $variables['show_separator'] = '';
    $variables['narrative'] = 'Program EPG Text';
    $variables['external_link'] = '';
    $variables['external_id'] = 'Program Episode Number';
    $variables['reuse_show'] = FALSE;
    $variables['channel_tid'] = 1686;
    $variables['airing_type'] = 969;
    $variables['limit_shows_per_day'] = TRUE;
  }
  else if ($import_type == 'nyxt') {
    $variables['suffix'] .= ' [NYXT]';
    $variables['project'] = 'Produced By (Organization Name)';
    $variables['duration'] = 'Length';
    $variables['date'] = 'Date';
    $variables['start'] = 'Start Time';
    $variables['time_type'] = 'milliseconds'; 
    $variables['show'] = 'Segment Title';
    $variables['short_description'] = 'Sub Title';
    $variables['show_separator'] = ' - ';
    $variables['narrative'] = '';
    $variables['external_link'] = 'Source';
    $variables['external_id'] = 'id';
    $variables['reuse_show'] = TRUE;
    $variables['channel_tid'] = 1689;
    $variables['airing_type'] = 969;
    $variables['limit_shows_per_day'] = FALSE;
  }
  else {
    drupal_set_message("Error Code 194683 cannot set variables for ".
		       $import_type, 'error');
  }
  return $variables;
}
/**
  * Helper function to get a single value off of a entity 
 */
function cma_program_import_get_single_field_value($entity, 
						   $field_name,
						   $index = 'value') {
  if (isset($entity->{$field_name})) {
    $field = $entity->{$field_name};
    if (isset($field[LANGUAGE_NONE]) &&
        isset($field[LANGUAGE_NONE][0]) &&
        isset($field[LANGUAGE_NONE][0][$index])) {
      return $field[LANGUAGE_NONE][0][$index];
    }
  }
  return NULL;
}

  
    
function cma_program_import_is_import_complete($node) {
  $airing_id =
    cma_program_import_get_single_field_value($node,
					      'field_uploader_airingref',
					      'target_id');


  $show_id =
    cma_program_import_get_single_field_value($node,
					      'field_uploader_showref',
					      'target_id');
  if ($airing_id || $show_id) {
    return TRUE;
  }

  return FALSE;
}


/**
 * PHP field for this view admin/structure/views/view/airing_detail_for_grid
 */
function cma_program_import_date_view_field($row) {
  $airing_id = $row->airing_id;
 
  $airings = entity_load('airing', array($airing_id));

  if ($airings) {
    $airing = array_shift($airings);
    if ($airing) {
      $start = $airing->field_airing_date[LANGUAGE_NONE][0]['value'];
      $start = strtotime($start);
      $start = date('h:i:sa', $start);
      
      $end = $airing->field_airing_date[LANGUAGE_NONE][0]['value2'];
      $end = strtotime($end);
      $end = date('h:i:sa', $end);

      return $start . " - " . $end;
    }
  }
  return '';
}

/**
 * View field used in the program_import_search view
 */
function cma_program_import_airing_range_view_field($row) {
  $import = node_load($row->nid);
  if (!$import) {
    return;
  }

  if ($import->field_uploader_airingref &&
      $import->field_uploader_airingref[LANGUAGE_NONE]) {

    $airings = $import->field_uploader_airingref[LANGUAGE_NONE];

    $first_id = $last_id = NULL;
    foreach($airings as $airing) {
      if (!$first_id) {
	$first_id = $airing['target_id'];
      }
      $last_id = $airing['target_id'];
    }

    $airings = entity_load('airing', array($first_id));

    if ($airings) {
      $first_airing = array_shift($airings);
    }

    $airings = entity_load('airing', array($last_id));

    if ($airings) {
      $last_airing = array_shift($airings);
    }
  }

  if ($first_airing) {
    $start = cma_program_import_get_single_field_value($first_airing,
						       'field_airing_date',
						       'value');
    $start = date('F d, Y h:ia', strtotime($start));
    $end = cma_program_import_get_single_field_value($last_airing,
						     'field_airing_date',
						     'value2');
    $end = date('F d, Y h:ia', strtotime($end));
	
    return $start . ' - ' . $end;
  }
  return "Rolled Back";
    
}