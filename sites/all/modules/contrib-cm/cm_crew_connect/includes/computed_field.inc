<?
/*
function computed_field_field_civicrm_event_link_display($field, $entity_field_item, $entity_lang, $langcode) {
 dsm($field, 'field');
dsm($entity_field_item, 'item');
dsm($entity, 'ent');  
  return l('hi mom', '');
  }*/

function computed_field_field_civicrm_event_link_compute(&$entity_field, 
							 $entity_type, 
							 $entity, $field, 
							 $instance, 
							 $langcode, $items) {

  //SEE IF WE HAVE A CIVI EVENT
  $event_id = 
    cm_crew_connect_get_single_field_value($entity,
					   'field_crew_civievent_id');
  
  $link = '';
  if ($event_id) {
    $query = array('id'=>$event_id,
		   'action'=>'update',
		   'reset'=>1);

    $link = l("Manage CiviCRM Event", 
	      'civicrm/event/manage/settings', array('query'=>$query));
  }

  $entity_field[0]['value'] = $link;
  return $link;
}