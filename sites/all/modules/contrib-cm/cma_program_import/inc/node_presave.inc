<?php
/**
 * presave hook that will 
 */
function cma_program_import_node_presave($node) {
  
  if ($node->type == 'program_import') {
    //there will be different import types based on FSTV vs Next vs ????
    $import_type = $node->field_uploader_type[LANGUAGE_NONE][0]['value'];

    $data_array = $_SESSION['cma_program_import_data_array'];

    //clear array from session
    $_SESSION['cma_program_import_data_array'] = array();

    $daily_premieres = array();
    if ($data_array) {
      dsm($data_array, 'my data array in inc/node_presave.inc');
    
      //loop through all the results from the data array
      for ($i = 0; $i < count($data_array); $i ++) {
	$values = $data_array[$i];



	
	//see if a show exists
	//TBD THIS IS NOT IMMPLEMENTED YET AS NOT YET PART OF WORKFLOW
	$show = cma_program_import_show_lookup($import_type,
					       $values);

	if (!$show) {
	  $show = $daily_premieres[$values['Schedule Item Log Date']]
	    [$values['project_nid']];
	  if (!$show) {
	    $show = cma_program_import_show_create($values);

	    if (!$show) {
	      drupal_set_message("Error Code 45392 No show returned!",'error');
	      continue;
	    }

	    $daily_premieres[$values['Schedule Item Log Date']]
	      [$values['project_nid']] = $show;
	  }
	}

	$values['show_nid'] = $show->nid;
      }

      dsm($daily_premieres, 'daily premieres');
    }

  }
}


