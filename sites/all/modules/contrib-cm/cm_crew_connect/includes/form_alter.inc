<?

/**
 * Implements hook_form_alter().
 */
function cm_crew_connect_form_alter(&$form, &$form_state, $form_id) {

  //////////////////////////////////////////////////////////////////////
  //The Crew Connect Request Node Form
  if ($form_id == 'crew_connect_request_node_form') {
    global $user;
    $author_id = $user->uid;

    $nid = (isset($form['nid']['#value'])) ? $form['nid']['#value'] : NULL;    
      
    // IF WE FOUND A nid ON THE FORM WE ARE EDITING THE REQUEST
    if ($nid) {
      $request = $form['field_crew_project_id'][LANGUAGE_NONE][0]
	['target_id']['#entity'];

      $project_id = 
 	cm_crew_connect_get_single_field_value($request, 
					       'field_crew_project_id',
					       'target_id');   
    }
    // NO NID ON FORM THEREFORE WE ARE CREATING A REQUEST
    else {
      //GET PROJECT FROM URL
      $project_id = isset($_GET['field_crew_project_id']) ? 
	$_GET['field_crew_project_id'] : NULL;

      if (!isset($project_id) || !$project_id) {
	return;
      }
    }
      
    $project = $project_id ? node_load($project_id) : NULL;            

    //IF WE ARE CREATING A NEW REQUEST, PREPOPULATE FIELDS
    if (!$request) {
      $form['field_crew_producer_name'][LANGUAGE_NONE][0]
	['value']['#default_value'] =
      cm_crew_connect_get_display_name($author_id);
    }

    //WE WILL LOOP THROUGH ALL THE FUNCTION THAT DEFINE OUR HOOK
    //WE AREN'T USING module_invoke or drupal_alter because we want to
    //alter an array AND pass in the crew request and author uid
    $hook_name = "crew_connect_request_form_alter";
    foreach (module_implements($hook_name) as $module) {
      $function = $module . '_' . $hook_name;
      $function($form, $request, $author_id);
    } 

  }

  //////////////////////////////////////////////////////////////////////
  //The Crew Connect Applicant Form
  if ($form_id == 'crew_connect_application_node_form') {
    //SET SUBMIT BUTTON TEXT
    $form['actions']['submit']['#value'] = t('Sign Up');
    
    global $user;
    $author_id = $user->uid;

    $nid = (isset($form['nid']['#value'])) ? $form['nid']['#value'] : NULL; 
    $application = $nid ? node_load($nid) : NULL;
    
    //WE WILL LOOP THROUGH ALL THE FUNCTION THAT DEFINE OUR HOOK
    //WE AREN'T USING module_invoke or drupal_alter because we want to
    //alter an array AND pass in the crew application and author uid
    $hook_name = "crew_connect_application_form_alter";
    foreach (module_implements($hook_name) as $module) {
      $function = $module . '_' . $hook_name;
      $function($form, $application, $author_id);
    } 
  }

  //////////////////////////////////////////////////////////////////////
  //The Crew Connect Request "Are you Sure?" Form
  if ($form_id == 'node_delete_confirm' && 
      $form['#node']->type == 'crew_connect_request') {
    $request = $form['#node'];
    
    //FIND A POSITION TO ADD TO MESSAGE PROMPT
    $position = '';
    
    //DOES THE NEW POSITION FIELD EXIST?
    $field = field_info_instance('node', 'field_crew_position_taxonomy',
				 'crew_connect_request');
    if ($field) {
      $tid =
	cm_crew_connect_get_single_field_value($request,
					       'field_crew_position_taxonomy',
					       'tid');
      
      $term = $language_tid ? taxonomy_term_load($tid) : NULL;
      $position = $term ? $term->name : '';
    }
    //ELSE, DOES THE LEGACY POSITION FIELD EXIST
    else {
      $field = field_info_instance('node', 'field_crew_positions',
				   'crew_connect_request');
      //LEGACY HAD POSITION IN TITLE, HACKY BUT EFFECTIVE
      //FIXME GRAB POSITION OFF $request, NOT FROM TITLE
      if ($field) {
	$position = $request ?
	  substr($request->title, strpos($request->title, ":") + 2) : '';  
	
      }
    }

    $text = "<h3>".
      t('Are you sure you want to Cancel this "!position" Crew Request?',
	array('!position' => $position))."</h3>";

    //UPDATE MESSAGE AND BUTTONS
    $form['description']['#markup'] = $text;
    $form['actions']['submit']['#value'] = 'Yes';
    $form['actions']['cancel']['#title'] = 'No';

  }
  //////////////////////////////////////////////////////////////////////
  //The Crew Connect Application "Are you Sure?" Form
  else if ($form_id == 'node_delete_confirm' && 
	   $form['#node']->type == 'crew_connect_application') {
    
    //UPDATE MESSAGE AND BUTTONS
    $form['description']['#markup'] =
      "<h3>".t('Are you sure you want to Cancel this Crew Sign Up?')."</h3>";
    
    $form['actions']['submit']['#value'] = 'Yes';
    $form['actions']['cancel']['#title'] = 'No';

  }

  //////////////////////////////////////////////////////////////////////
  // REQUEST SEARCH FORM FOR PRODUCERS
  // CONVERTS TEXT FIELD TO DROPDOWN FIELD
  // https://www.drupal.org/node/1549250
  if ($form['#id'] == 'views-exposed-form-crew-connect-search-page') {
    $form['submit']['#value'] = "Search";

    //CALLS HOOKS TO MAKE CUSTOM IMPROVEMENTS TO THE SEARCH FORM
    drupal_alter("crew_connect_search_form_alter", $form);
  }    
}

