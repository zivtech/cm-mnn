<?php

/**
 * validate hookd of cma_program_import_form
 */
function cma_program_import_node_validate($node, $form, &$form_state){

  if ($node->type == 'program_import') {
    if (cma_program_import_is_import_complete($node)) {
      //do nothing
      return;
    }

    //there will be different import types based on FSTV vs Next vs ????
    $import_type = $node->field_uploader_type[LANGUAGE_NONE][0]['value'];

    //this will load data from the file field and put it into a array
    $data_array = cma_program_import_process_csv($node);
    
    //if we didn't get an array from a file field, throw error
    if (!$data_array) {
      $err_msg = "Error Code #8712 No data found from import file";
      form_set_error('field_uploader_file', $err_msg);
      return;
    }
    
    
    //loop through all the results from the data array
    $projects = array();
    $project_errors = array();
    foreach($data_array as $key => $values) {
      $title = $values['Program EPG Title'];

      if ($title == 'The Thom Hartmann Program' ||
	  $title == 'The Stephanie Miller Show') {
	$data_array[$key]['project_show_merge'] = 'true';
      }
      if ($import_type == 'fstv') {
	$title .= ' [FSTV]';
      }
      
      $project = $projects[$title];

      if (!$project) {
	$project = cma_program_import_project_lookup($import_type, $title);

	if (!$project) {
	  $err_msg .=
	    "<br/>".$title.' does not exist, you must create it first.';
	  $project = new stdClass();
	  $project->title = $title;
	  $project->nid = NULL;
	}	
	$projects[$title] = $project;
      }

      $data_array[$key]['project_nid'] = $project->nid;
   
    }

    if ($err_msg) {
      form_set_error('field_uploader_file', $err_msg);
      return;
    }

    //we have successfully processed data
    //lets store it in a session variable for later
    $_SESSION['cma_program_import_data_array'] = $data_array;
    

  }  
}

