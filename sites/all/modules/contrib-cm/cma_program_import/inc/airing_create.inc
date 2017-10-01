<?php

function cma_program_import_airing_create($row, $variables) {
  $airing = entity_create('airing', array());
  $start_time = strtotime($row[$variables['date']] . ' 00:00:00');
  $start_offset = $row[$variables['start']];
  
  if ($variables['time_type'] == 'milliseconds') {
    $start_offset = floor($start_offset/1000);
  }
  else if ($variables['time_type'] == 'h:m:s') {
    $start_offset = strtotime("1970-01-01 " . $start_offset  ." UTC");
  }
  
  $start_time += $start_offset;
  $end_time = $start_time + $row['duration_in_seconds'];
    
  $airing->field_airing_date[LANGUAGE_NONE][0]['value'] =
    date('Y-m-d H:i:s', $start_time);

  $airing->field_airing_date[LANGUAGE_NONE][0]['value2'] =
    date('Y-m-d H:i:s', $end_time);

  $airing->field_airing_project_ref[LANGUAGE_NONE][0]['target_id'] =
    $row['project_nid'];

  $airing->field_airing_show_ref[LANGUAGE_NONE][0]['target_id'] =
    $row['show_nid'];

  $airing->field_airing_channel[LANGUAGE_NONE][0]['tid']  =
    $variables['channel_tid'];
  
  $airing->field_airing_type[LANGUAGE_NONE][0]['tid'] =
    $variables['airing_type'];  

  $airing->cm_use_auto_title = TRUE;
  entity_save('airing', $airing);
  $airing->airing_id = entity_id('airing', $airing);

  return $airing;
}
