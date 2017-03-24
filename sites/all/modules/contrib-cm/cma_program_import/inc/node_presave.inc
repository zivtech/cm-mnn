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
    $skipped_rows_array = array();
    if ($data_array) {
      //loop through all the results from the data array
      for ($i = 0; $i < count($data_array); $i ++) {
	$row = $data_array[$i];
	$row_index = $i;
	$row_duration = strtotime("1970-01-01 " .$row['Schedule Item Duration']
				  ." UTC");
		
	if ($row['project_show_merge']) {
	  
	  for ($j = $i + 1; $j < count($data_array); $j ++) {
	    //look at next row
	    $next_row = $data_array[$j];
	    if ($next_row['project_nid'] == $row['project_nid'] &&
		$row['project_show_merge'] == 'true') {

	      $skipped_rows_array[] = $next_row;
	      $i ++;

	      $next_row_duration =
		strtotime("1970-01-01 " .$next_row['Schedule Item Duration']
			  ." UTC");

	      //update duration of show
	      //FIXME SOMETHING WRONG HERE
	      $row_duration += $next_row_duration;
	    }
	    else {
	      break;
	    }
	  }
	  $row['Schedule Item Duration'] = gmdate('H:i:s', $row_duration);
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

	    //FIXME SET AIRING ID ON THE IMPORT NODE
	    $node->field_uploader_showref =
	      $node->field_uploader_showref ?
	      $node->field_uploader_showref : array();

	    $node->field_uploader_showref[LANGUAGE_NONE] =
	      $node->field_uploader_showref[LANGUAGE_NONE] ?
	      $node->field_uploader_showref[LANGUAGE_NONE] : array();
	    
	    $node->field_uploader_showref[LANGUAGE_NONE][] =
	      array('target_id' => $show->nid);
	    
	    if (!$show) {
	      drupal_set_message("Error Code 45392 No show returned!",'error');
	      continue;
	    }

	    $daily_premieres[$row['Schedule Item Log Date']]
	      [$row['project_nid']] = $show;
	  }
	}

	$row['show_nid'] = $show->nid;

	$airing = cma_program_import_airing_create($row);

	if (!$airing) {
	  drupal_set_message("Error Code 45393 No airing returned!",'error');
	  continue;
	}
	  
	$row['airing_id'] = $airing->airing_id;
	
	//FIXME SET AIRING ID ON THE IMPORT NODE
	$node->field_uploader_airingref =
	  $node->field_uploader_airingref ?
	  $node->field_uploader_airingref : array();
	
	$node->field_uploader_airingref[LANGUAGE_NONE] =
	  $node->field_uploader_airingref[LANGUAGE_NONE] ?
	  $node->field_uploader_airingref[LANGUAGE_NONE] : array();

	$node->field_uploader_airingref[LANGUAGE_NONE][] =
	  array('target_id' => $airing->airing_id);

	$post_import_array[] = $row;
      }

      /**
      dsm($node, 'node');
      dsm($daily_premieres, 'daily premieres');
      dsm($post_import_array, 'modified data array');
      dsm($skipped_rows_array, 'skipped rows');
      **/
    }

  }
}


