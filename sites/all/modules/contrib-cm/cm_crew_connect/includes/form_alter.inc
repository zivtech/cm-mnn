<?

/**
 * Implements hook_form_alter().
 */
function cm_crew_connect_form_alter(&$form, &$form_state, $form_id) {

  //The Crew Connect Request Node Form
  if ($form_id == 'crew_connect_request_node_form') {
    drupal_set_message(t("Please make sure you have a Confirmed Reservation for the relevant facility and time prior to asking for a Crew."), 'warning');
    $project_nid = filter_input(INPUT_GET, 'field_crew_project_id');
    $project = $project_nid ? node_load($project_nid) : NULL;

    if (!$project && $form['nid']) {
      $request = 
	$form['field_crew_project_id'][LANGUAGE_NONE][0]
	['target_id']['#entity'];

      $project_nid = 
	cm_crew_connect_get_single_field_value($request, 
					       'field_crew_project_id',
					       'target_id');
      $project = $project_nid ? node_load($project_nid) : NULL;
    }

    $author = ($project && $project->uid) ? user_load($project->uid) :
      NULL;

    $name = cm_crew_connect_get_display_name($author->uid);

    $form['field_crew_producer_name'][LANGUAGE_NONE][0]
      ['value']['#default_value'] = $name;

    $form['field_crew_public_description'][LANGUAGE_NONE][0]
      ['value']['#default_value'] = 
      cm_crew_connect_get_single_field_value($project, 'field_description');

    $form['field_crew_public_url'][LANGUAGE_NONE][0]
      ['value']['#default_value'] = 
      cm_crew_connect_get_single_field_value($project, 
					     'field_project_public_url');

    $language_tid = 
      cm_crew_connect_get_single_field_value($project, 
					     'field_pbcore_languages',
					     'tid');
    $term = $language_tid ? taxonomy_term_load($language_tid) : NULL;

    $term_name = $term ? $term->name : "";
    if ($term_name) {
      $form['field_crew_language'][LANGUAGE_NONE][0]
	['value']['#default_value'] = $term_name;
    }

    $form['field_crew_producer_name'][LANGUAGE_NONE]['#disabled'] = TRUE;
    $form['field_crew_public_description'][LANGUAGE_NONE]['#disabled'] = TRUE;
    $form['field_crew_public_url'][LANGUAGE_NONE]['#disabled'] = TRUE;
    $form['field_crew_language'][LANGUAGE_NONE]['#disabled'] = TRUE;
    
    if ($project) {
      $form['title'] = array('#type'=>'hidden',
			     '#value'=>$project->title);
    }

    $form['field_crew_start_time']['#prefix'] = "<table><tr><td>";
    $form['field_crew_start_time']['#suffix'] = "</td>";
    $form['field_crew_length_of_time']['#prefix'] = "<td>";
    $form['field_crew_length_of_time']['#suffix'] = "</td></tr></table>";
    if (!$form['nid']['#value']) {
      $form['actions']['submit']['#value'] = t('Create Crew Request');
    }

  }

  //The Crew Connect Applicant Form
  if ($form_id == 'crew_connect_application_node_form') {
    $form['actions']['submit']['#value'] = t('Sign Up');
  }

  if ($form['#action'] == '/crew-connect/requests-search') {
    $form['title']['#description'] = 
      'This only shows Projects that have active Crew Requests.';
    $form['submit']['#value'] = "Search";
  }

  if ($form_id == 'node_delete_confirm' && 
      $form['#node']->type == 'crew_connect_request') {
    $nid = arg(1);
    $node = $nid ? node_load($nid) : NULL;
    $type = $node ? substr($node->title, strpos($node->title, ":") + 2) : '';  

    $text = "<h3>".
      t('Are you sure you want to Cancel this "!type" Crew Request?',
	array('!type' => $type))."</h3>";
    
    $form['description']['#markup'] = $text;
    $form['actions']['submit']['#value'] = 'Yes';
    $form['actions']['cancel']['#title'] = 'No';

  }
  else if ($form_id == 'node_delete_confirm' && 
      $form['#node']->type == 'crew_connect_application') {
    $text = "<h3>".
      t('Are you sure you want to Cancel this Crew Sign Up?',
	array('!type' => $type))."</h3>";
    
    $form['description']['#markup'] = $text;
    $form['actions']['submit']['#value'] = 'Yes';
    $form['actions']['cancel']['#title'] = 'No';

  }
}

