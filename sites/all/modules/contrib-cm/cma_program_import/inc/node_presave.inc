<?php
/**
 * presave hook that will 
 */
function cma_program_import_node_presave($node) {
  
  if ($node->type == 'program_import') {
    if (cma_program_import_is_import_complete($node)) {
      //do nothing
      return;
    }
    //there will be different import types based on FSTV vs Next vs ????
    $import_type = $node->field_uploader_type[LANGUAGE_NONE][0]['value'];

    $data_array = $_SESSION['cma_program_import_data_array'];

    //clear array from session
    $_SESSION['cma_program_import_data_array'] = array();

    $daily_premieres = array();
    $post_import_array = array();
    if ($data_array) {
      dsm($data_array, 'my data array in inc/node_presave.inc');
    
      //loop through all the results from the data array
      for ($i = 0; $i < count($data_array); $i ++) {
	$row = $data_array[$i];
	$row_index = $i;
	if ($row['project_show_merge']) {
	  
	  //for ($j = $i; $j < count($data_array); $j ++) {
	    //look at next row
	    
	  //}
	}

	//see if a show exists
	//TBD THIS IS NOT IMMPLEMENTED YET AS NOT YET PART OF WORKFLOW
	$show = cma_program_import_show_lookup($import_type,
					       $row);

	if (!$show) {
	  $show = $daily_premieres[$row['Schedule Item Log Date']]
	    [$row['project_nid']];
	  if (!$show) {
	    $show = cma_program_import_show_create($row);

	    if (!$show) {
	      drupal_set_message("Error Code 45392 No show returned!",'error');
	      continue;
	    }

	    $daily_premieres[$row['Schedule Item Log Date']]
	      [$row['project_nid']] = $show;
	  }
	}

	$row['show_nid'] = $show->nid;

	$post_import_array[] = $row;
      }

      dsm($daily_premieres, 'daily premieres');
      dsm($post_import_array, 'modified data array');
    }

  }
}


