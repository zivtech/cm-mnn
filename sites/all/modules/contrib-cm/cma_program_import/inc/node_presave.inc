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
    $variables = cma_program_import_get_variables($import_type);
    
    //clear array from session
    $_SESSION['cma_program_import_data_array'] = array();

    $daily_premieres = array();
    $post_import_array = array();
    $skipped_rows_array = array();
    if ($data_array) {
      //loop through all the results from the data array
      for ($i = 0; $i < count($data_array); $i ++) {
	$row = $data_array[$i];

	$row_duration = $row[$variables['duration']];
	if ($variables['time_type'] == 'milliseconds') {
	  $row_duration = floor($row_duration/1000);
	}
	else if ($variables['time_type'] == 'h:m:s') {
	  $row_duration = strtotime("1970-01-01 " . $row_duration  ." UTC");
	}

	if ($row['project_show_merge']) {
	  
	  for ($j = $i + 1; $j < count($data_array); $j ++) {
	    //look at next row
	    $next_row = $data_array[$j];
	    if ($next_row['project_nid'] == $row['project_nid'] &&
		$row['project_show_merge'] == 'true') {

	      $skipped_rows_array[] = $next_row;
	      $i ++;

	      $next_row_duration = $next_row[$variables['duration']];

	      if ($variables['time_type'] == 'milliseconds') {
		$next_row_duration = floor($next_row_duration/1000);
	      }
	      else if ($variables['time_type'] == 'h:m:s') {
		$next_row_duration =
		  strtotime("1970-01-01 " . $next_row_duration  ." UTC");
	      }
	      //update duration of show
	      $row_duration += $next_row_duration;
	    }
	    else {
	      break;
	    }
	  }
	}
	
	$row[$variables['duration']] = gmdate('H:i:s', $row_duration);
	$row['duration_in_seconds'] = $row_duration;
	
	//see if a show exists
	$show = cma_program_import_show_lookup($variables,
					       $row);
	if (!$show) {
	  $date = $row[$variables['date']];
	  if ($variables['limit_shows_per_day']) {
	    $show = $daily_premieres[$date][$row['project_nid']];
	  }

	  if (!$show) {
	    $show = cma_program_import_show_create($row, $variables);
	    
	    if (!$show) {
	      drupal_set_message("Error Code 45392 No show returned!",'error');
	      continue;
	    }
	    $node->field_uploader_showref =
	      $node->field_uploader_showref ?
	      $node->field_uploader_showref : array();

	    $node->field_uploader_showref[LANGUAGE_NONE] =
	      $node->field_uploader_showref[LANGUAGE_NONE] ?
	      $node->field_uploader_showref[LANGUAGE_NONE] : array();
	    
	    $node->field_uploader_showref[LANGUAGE_NONE][] =
	      array('target_id' => $show->nid);
	    
	    if ($variables['limit_shows_per_day']) {
	      $daily_premieres[$date][$row['project_nid']] = $show;
	    }
	  }
	}
	
	$row['show_nid'] = $show->nid;
	$airing = cma_program_import_airing_create($row, $variables);

	if (!$airing) {
	  drupal_set_message("Error Code 45393 No airing returned!",'error');
	  continue;
	}
	  
	$row['airing_id'] = $airing->airing_id;
	
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
    }
  }
}


