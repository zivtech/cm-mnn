<?php

function cma_program_import_show_create($row, $variables) {
  global $user;

  $node = new stdClass();
  $node->type = "cm_show";
  node_object_prepare($node);
  
  $node->title = "";
  $node->uid = $user->uid;
  $node->language = LANGUAGE_NONE;

  //Show description/narrative
  $narrative = $row[$variables['narrative']];
  if ($narrative) {
    $node->field_narrative[$node->language][0]['value'] = $narrative;
  }

  $short_description = $row[$variables['short_description']];
  if ($short_description) {
    $node->field_short_description[$node->language][0]['value'] =
      $short_description;
  }

  //Project NID
  $node->og_node2[$node->language][0]['target_id'] = $row['project_nid'];

  //External Episode Number
  $external_id = $row[$variables['external_id']];
  if ($external_id) {
    $node->field_external_episode_number[$node->language][0]['value'] =
      $external_id;
  }

  //External Episode Number
  $external_link = $row[$variables['external_link']];
  if ($external_link) {
    $node->field_external_link[$node->language][0]['value'] = $external_link;
  }

  //Access Center Episode Number
  $node->field_episode_number[$node->language][0]['value'] =
    mnn_show_get_next_episode_number($row['project_nid']);

  //Schedule Type
  $node->field_taxonomy_cm_project_types[LANGUAGE_NONE][0]['tid'] = 1;
    
  //Format
  $node->field_format[LANGUAGE_NONE][0]['tid'] = 1062;
  
  //Runtime
  $node->field_runtime[LANGUAGE_NONE][0]['tid'] = 854;
    
  //Timecode In
  $node->field_timecode_in[LANGUAGE_NONE][0]['value'] = 0;

  //Timecode Out
  $node->field_timecode_out[LANGUAGE_NONE][0]['value'] =
    strtotime("1970-01-01 " .$row['Schedule Item Duration']." UTC");
  
  //Language
  $node->field_pbcore_languages[LANGUAGE_NONE][0]['tid'] = 231;
    
  //Rating
  $node->field_pbcore_fcc_ratings[LANGUAGE_NONE][0]['tid'] = 718;

  $node= node_submit($node);
  node_save($node);
      
  return $node;  
}