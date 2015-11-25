<?
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

function cm_crew_connect_role_position_mapping() {
  $ret = array(
	       'admin' => array(
				"expess_studios",
				"editor_premiere",
				"studios_audio",
				"studios_cameras",
				"studios_director",
				"studios_expression_cg",
				"studios_floor_manager",
				"studios_general_prod_asst",
				"studios_switcher",
				"studios_teleprompter",
				"studios-robotic",
				"editor-xpression-cg",
				),
	       'Staff - All' => array(
				      "expess_studios",
				      "editor_premiere",
				      "studios_audio",
				      "studios_cameras",
				      "studios_director",
				      "studios_expression_cg",
				      "studios_floor_manager",
				      "studios_general_prod_asst",
				      "studios_switcher",
				      "studios_teleprompter",
				      "studios-robotic",
				      "editor-xpression-cg",

				      ),
	       'PREM' => array(
			       "editor_premiere",
			       ),
	       
	       'DSTE' => array(
			       "expess_studios",
			       ),
	       'DSTW' => array(
			       "expess_studios",
			       "studios_audio",
			       "studios_cameras",
			       "studios_director",
			       "studios_expression_cg",
			       "studios_floor_manager",
			       "studios_general_prod_asst",
			       "studios_switcher",
			       "studios_teleprompter",
			       "studios-robotic",
			       "editor-xpression-cg",

			       ),
	       );
  return $ret;
}

function cm_crew_connect_role_positions($mapping) {
  $positions = NULL;
  $is_first = TRUE;
  $sql = "SELECT name from {role}";
  $roles_result = db_query($sql);
  
  while($role = $roles_result->fetchObject()) {
    if (!cm_crew_connect_user_has_role(array($role->name))) {
      continue;
    }

    if (isset($mapping[$role->name])) {
      foreach($mapping[$role->name] as $position) {
	if (!$is_first) {
	  $positions .= "+";
	}
	else {
	  $is_first = FALSE;
	}
  
	$positions .= $position;
      }
    }
  }
  return $positions;
}

function cm_crew_connect_other_crew_producers() {
  global $user;
  $sql = "SELECT uid FROM node WHERE type = 'crew_connect_request'";
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

function cm_crew_connect_override_messages() {
  $messages = drupal_get_messages();
  foreach($messages as $type=>$type_msgs) {
    foreach ($type_msgs as $message) {
      if (strpos($message, 'rew Connect Request') &&
	  strpos($message, 'has been deleted') ) {
	$message = t("You have successfully cancelled your Crew Request.");

      }
      else if (strpos($message, 'rew Connect Application') &&
	       strpos($message, 'has been deleted') ) {
	$message = t("You have successfully cancelled your Crew Sign Up.");
      }
      else if (strpos($message, 'rew Connect Application') &&
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