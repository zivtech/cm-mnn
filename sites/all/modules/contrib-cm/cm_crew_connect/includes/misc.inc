
<?
/**
 * File will return the label for the Position on the Crew Connect
 * It will look at both the current field for position, as well as the 
 * legacy
 */
function cm_crew_connect_get_position_label(&$request) {
  $position = array();
  
  //DOES THE NEW POSITION FIELD EXIST?
  $position_field = field_info_instance('node','field_crew_position_taxonomy',
					 'crew_connect_request');
  if ($position_field) {
    $tid =
      cm_crew_connect_get_single_field_value($request,
					     'field_crew_position_taxonomy',
					     'tid');
    
    $term = $tid ? taxonomy_term_load($tid) : NULL;
    $position['id'] = $tid;
    $position['label'] = $term ? $term->name : '';
  }
  //ELSE, DOES THE LEGACY POSITION FIELD EXIST
  else {
    $position_field = field_info_instance('node', 'field_crew_positions',
					  'crew_connect_request');
    if ($position_field) {
      $value =
	cm_crew_connect_get_single_field_value($request,
					       'field_crew_positions',
					       'value');
      $position_field = field_info_field('field_crew_positions');   
      $allowed_values = list_allowed_values($position_field);
      
      $position['id'] = $value;
      $position['label'] =
	$allowed_values[$value] ? $allowed_values[$value] : '';

    }
  }
   
  return $position;
}

/**                                                                            
 * Helper function to get a single value off of a entity  
*/
function cm_crew_connect_get_single_field_value($entity, $field_name,
						$index = 'value') {
  if (isset($entity->{$field_name})) {
    $field = $entity->{$field_name};
    if (isset($field[LANGUAGE_NONE]) &&
        isset($field[LANGUAGE_NONE][0]) &&
        isset($field[LANGUAGE_NONE][0][$index])) {
      return $field[LANGUAGE_NONE][0][$index];
    }
  }
  return NULL;
}

/**                                                         
 * Checks if the current user has a role
 *  
 * @param array $roles
 *
 * @return bool
 */ 
function cm_crew_connect_user_has_role($roles = array()) {
  global $user;
  foreach ($roles as $role) { 

    if (in_array($role, $user->roles)) {
      return TRUE;
    }
  }
  return FALSE;
}           
/**
 * Function will return all the possible Position names on a Crew Request
 *
 * @return $positions - '+' deliminated string of position tid's or values
 */
function cm_crew_connect_get_default_position_arg() {
  $position_arg = '';
  
  //DOES THE NEW POSITION FIELD EXIST?
  $position_field = field_info_instance('node','field_crew_position_taxonomy',
					 'crew_connect_request');
  if ($position_field) {
    //get all taxonomy terms
    $v = taxonomy_vocabulary_machine_name_load('Crew Connect Positions');
    $terms = taxonomy_get_tree($v->vid);
    $is_first = TRUE;
    foreach ($terms as $term) {
      if (!$is_first) {
	$position_arg .= "+";
      }
      else {
	$is_first = FALSE;
      }
      $position_arg .= $term->tid;
    }
  }
  //ELSE, DOES THE LEGACY POSITION FIELD EXIST
  else {
    $position_field = field_info_instance('node', 'field_crew_positions',
					  'crew_connect_request');
    if ($position_field) {
      $position_field = field_info_field('field_crew_positions');   
      $allowed_values = list_allowed_values($position_field);

      $is_first = TRUE;
      foreach($allowed_values as $value => $label) {
	if (!$value) {
	  continue;
	}
	if (!$is_first) {
	  $position_arg .= "+";
	}
	else {
	  $is_first = FALSE;
	}
	$position_arg .= $value;
      }
    }
  }
  return $position_arg;
}

/**
 * Finds all authors of Crew Requests other than the logged in user
 */
function cm_crew_connect_other_crew_producers() {
  global $user;
  $sql = "SELECT distinct uid FROM node WHERE type = 'crew_connect_request'";
  $results = db_query($sql);

  $users =  "";
  $is_first = TRUE;
  while ($result = $results->fetchObject()) {
    if ($user->uid == $result->uid) {
      continue;
    }
    if (!$is_first) {
      $users .= "+";
    }
    else {
      $is_first = FALSE;
    }
    $users .= $result->uid;
  }
  return $users;
}

/**
 * Function gets the set of current drupal status messages and updates them
 * so that the status messages make more sense for crew connect
 */
function cm_crew_connect_override_messages() {
  $messages = drupal_get_messages();
  foreach($messages as $type=>$type_msgs) {
    foreach ($type_msgs as $message) {
      if (strpos($message, 'Crew Connect Request') &&
	  strpos($message, 'has been deleted') ) {
	$message = t("You have successfully cancelled your Crew Request.");

      }
      else if (strpos($message, 'Crew Connect Application') &&
	       strpos($message, 'has been deleted') ) {
	$message = t("You have successfully cancelled your Crew Sign Up.");
      }
      else if (strpos($message, 'Crew Connect Application') &&
	  strpos($message, 'has been created') ) {
	$node = $_SESSION['cm_crew_connect_new_application'];
	$_SESSION['cm_crew_connect_new_application'] = NULL;
	
	$req = 
	  cm_crew_connect_get_single_field_value($node, 
						 'field_app_crew_connect_node',
						 'target_id');

	$req = $req ? node_load($req) : NULL;

	$project_nid = $req ? 
	  cm_crew_connect_get_single_field_value($req,
						 'field_crew_project_id',
						 'target_id') : NULL;

	$project_title = $project_nid ? node_load($project_nid)->title : "";
	
	$date = $req ? 
	  cm_crew_connect_get_single_field_value($req,
						 'field_crew_start_date') 
	  : NULL;
	$date = date('m/d/Y', strtotime($date));

	$message = 
	  t("You have successfully signed up to crew for !proj on !date.",
	    array('!proj'=>$project_title, '!date'=>$date));

      }

      drupal_set_message($message, $type);
    }
  }
}

/**
 * Function returns an Application node for a nid of a Crew Request node
 */
function cm_crew_connect_get_application_for_request($nid) {
  $sql =
    "SELECT entity_id FROM {field_data_field_app_crew_connect_node}         
     WHERE field_app_crew_connect_node_target_id = :nid";
  $results = db_query($sql, array(':nid' => $nid));
  $result = $results->fetchObject();
  if ($result->entity_id) {
    $application = node_load($result->entity_id);
    return $application;
  }
  return NULL;
}