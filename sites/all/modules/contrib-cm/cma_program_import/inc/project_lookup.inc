<?php

function cma_program_import_project_lookup($title) {

  $sql = "SELECT nid FROM {node} where title = :title and type='cm_project'";
  
  $results = db_query($sql, array(':title' => $title));
  $result = $results->fetchObject();
  
  if ($result) {
    return node_load($result->nid);
  }
  return NULL;    
}
