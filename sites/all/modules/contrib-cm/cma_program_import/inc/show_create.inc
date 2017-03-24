<?php

function cma_program_import_show_create($row) {
  global $user;

  $node = new stdClass();
  $node->type = "cm_show";
  node_object_prepare($node);
  
  $node->title    = "";
  $node->uid = $user->uid;
  $node->language = LANGUAGE_NONE;
  
  $node->field_narrative[$node->language][0]['value'] =
    $row['Program EPG Text'];
  
  $node->og_node2[$node->language][0]['target_id'] = $row['project_nid'];

  //look up the episode number 
  $node->field_external_episode_number[LANGUAGE_NONE][0]['value'] =
    $row['Program Episode Number'];

  $node->field_episode_number[$node->language][0]['value'] =
    mnn_show_get_next_episode_number($row['project_nid']);

  $node= node_submit($node);
  node_save($node);
      
  //get id of node created 
  $new_node_id = $node->nid;
  
  return $node;  
}