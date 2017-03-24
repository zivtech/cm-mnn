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
  //FIXME ADD AN IF HERE TO SEE IF THERE ARE ALREADY AIRINGS SCHEDULED
  //IF THERE ARE AIRINGS RETURN FALSE

  $airing_id =
    cma_program_import_get_single_field_value($node,
					      'field_uploader_airingref',
					      'target_id');
  if ($airing_id) {
    return TRUE;
  }

  return FALSE;
}