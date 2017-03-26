<?php
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