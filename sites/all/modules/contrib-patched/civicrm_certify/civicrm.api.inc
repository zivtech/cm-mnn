<?php
/**
 * returns all active civi groups
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_get_civigroups() {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'is_active' => 1,
     // state explicitly since default index differs in Civi versions
    'sequential' => 0,
    'parents' => 44,    // only those in Certifications parent group
    // otherwise it will be limited to 25
    'options' => array(
        'limit' => 1000, 
    ),
  );

  $result = civicrm_api( 'group','get',$params );
 
  return $result;
}

/**
 * returns a single civi group
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_get_civigroup($group_id) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'is_active' => 1,
    'id' => $group_id,
  );

  $result = civicrm_api( 'group','getsingle',$params );

  return $result;
}

/**
 * returns all active participant status option values
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_get_particpant_status() {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'is_active' => 1,
  );

  $result = civicrm_api( 'ParticipantStatusType','get',$params );

  return $result;
}

/**
 * returns all active participant status option values
 * using the CiviCRM API v.3
 *
 * @params:
 *  $psid - the participant status id
 */
function _civicrm_certify_get_particpant_status_type($psid) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'is_active' => 1,
    'id' => $psid,
  );

  $result = civicrm_api( 'ParticipantStatusType','getsingle',$params );

  return $result;
}

/**
 * returns participants of a civievent
 * using the CiviCRM API v.3
 *
 * @params - an array of parameters to evaluate
 *
 */
function _civicrm_certify_get_particpants($event_id, $status_id) {
  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'event_id' => $event_id,
    'is_active' => 1,
    //'participant_status_id' => $status_id,
    'version' => 3,
    'option.limit' => 5000, // without this the limit is 25
  );
  $result = civicrm_api( 'Participant','get',$params );
  return $result;
}

/**
 * returns a single participant object
 * using the CiviCRM API v.3
 *
 * @params - an array of parameters to evaluate
 *
 */
function _civicrm_certify_get_particpant($participant_id) {
  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'id' => $participant_id,
    'version' => 3,
  );
  $result = civicrm_api( 'Participant','getsingle',$params );
  return $result;
}

/**
 * returns a signle contact
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_get_contact($contact_id) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'contact_id' => $contact_id,
  );
  $result = civicrm_api( 'contact','getsingle',$params );
  return $result;
}

/**
 * adds a civi contact to a group
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_group_contact($group_id, $contact_id) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'contact_id' => $contact_id,
    'group_id' => $group_id,
    'version' => 3,
  );

  $result = civicrm_api( 'group_contact','create',$params );

  return $result;
}

/**
 * adds a civi contact to a group
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_group_contact_delete($group_id, $contact_id) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'contact_id' => $contact_id,
    'group_id' => $group_id,
    'version' => 3,
  );

  $result = civicrm_api( 'group_contact','delete',$params );

  return $result;
}

/**
 * returns all active civi events
 * using the CiviCRM API v.3
 *
 * optionally returns only events of a specific type
 *
 */
function _civicrm_certify_get_civievents($event_type_id = NULL) {
  if (!civicrm_initialize()) {
    return null;
  }

  if ($event_type_id) {
    $params = array(
      'version' => 3,
      'is_active' => 1,
      'event_type_id' => $event_type_id,
      'option.limit' => 5000,
    );
  } else {
    $params = array(
      'version' => 3,
      'is_active' => 1,
      'option.limit' => 5000,
    );
  }

  $result = civicrm_api( 'event','get',$params );
  return $result;
}

/**
 * returns a single event
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_get_event($event_id) {
  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'event_id' => $event_id,
  );

  $result = civicrm_api( 'event','getsingle',$params );
  return $result;
}

/**
 * returns a list of values from an option_value list
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_get_options() {
  if (!civicrm_initialize()) {
    return null;
  }
  // get the option_group_id of event_type
  $option = _civicrm_certify_civicrm_get_option_id('event_type');

  $params = array(
    'version' => 3,
    'option_group_id' => $option['id'],
    'option.limit' => 500, // without this the limit is 25
  );

  $result = civicrm_api( 'OptionValue','get',$params );
  return $result;
}


/**
 * returns a option_value ID
 * using the CiviCRM API v.3
 *
 */
function _civicrm_certify_civicrm_get_option_id($option) {
  if (!civicrm_initialize()) {
    return null;
  }
  // get the option_group_id of event_type

  $params = array(
    'version' => 3,
    'name' => $option,
  );

  $result = civicrm_api( 'OptionGroup','get',$params );
  return $result;
}

/**
 * Lookup a contact from their Drupal id
 *
 *
 */
function _civicrm_certify_civicrm_ufmatch($account) {

  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'version' => 3,
    'uf_id' => $account,
  );
  $result = civicrm_api( 'UFMatch','getsingle',$params);
  return $result;
}

/**
 * creates a custom field group
 * using the CiviCRM API v.3
 *
 * kept for backward compatability
 */
function _civicrm_certify_create_custom_field_group($field_group_name) {
  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'title' => $field_group_name,
    'name' => $field_group_name,
    'extends' => array(
      '0' => 'Event',
    ),
    'style' => 'Inline',
    'is_active' => 1,
    'version' => 3,
  );

  $result = civicrm_api( 'custom_group','create',$params );

  return $result;
}


/**
 * creates a custom field whithin a field group
 * using the CiviCRM API v.3
 *
 * kept for backward compatability
 *
 */
function _civicrm_certify_create_civicrm_custom_field($field_name, $group_id, $option_id) {

  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'name' => $field_name,
    'label' => 'Certifications',
    'custom_group_id' => $group_id, //field group
    "option_group_id" => $option_id, //options
    'html_type' => 'Multi-Select',
    'data_type' => 'String',
    'weight' => 1,
    'text_length' => '255',
    'is_required' => 0,
    'is_searchable' => 0,
    'is_active' => 1,
    'version' => 3,
  );

  $result = civicrm_api('custom_field', 'create', $params);
  return $result;

}

/**
 * helper function providing option values for
 * _civicrm_certify_create_civicrm_custom_field
 * using the CiviCRM API v.3
 *
 * kept for backward compatability
 *
 */
function _civicrm_certify_create_civicrm_options(){

  if (!civicrm_initialize()) {
    return null;
  }

  $params = array(
    'version' => 3,
    'sequential' => 1,
    'name' => 'civicrm_certify.options',
    'is_reserved' => 1,
    'is_active' => 1,
  );

  $result = civicrm_api( 'option_group','create',$params );

  return $result;
}

/**
 * delete a custom field
 * using the CiviCRM API v.3
 *
 * kept for backward compatability
 *
 */
function _civicrm_certify_civicrm_delete_custom_field($field_id) {
  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'version' => 3,
    'id' => $field_id,
  );
  $result = civicrm_api( 'custom_field','delete',$params );
  return $result;
}

/**
 * delete a custom field group
 * using the CiviCRM API v.3
 *
 * kept for backward compatability
 *
 */
function _civicrm_certify_civicrm_delete_field_group($group_id) {
  if (!civicrm_initialize()) {
    return null;
  }
  $params = array(
    'version' => 3,
    'id' => $group_id,
  );
  $result = civicrm_api( 'custom_group','delete',$params);
  return $result;
}
