<?php  
function cma_program_import_show_lookup($variables, $row) {

  $show_node = NULL;
  $id = $row[$variables['external_id']];

  if ($id && $variables['reuse_show']) {
    $sql =
      "SELECT entity_id ".
      "FROM field_data_field_external_episode_number " .
      "WHERE field_external_episode_number_value = :id";

    $params = array(':id' => $id);
    $show_nid = db_query($sql, $params)->fetchField();

    if ($show_nid) {
      $show_node = node_load($show_nid);
    }
  }
  
  return $show_node;
}
