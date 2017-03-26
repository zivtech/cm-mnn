<?php

function cma_program_import_airing_create($row) {
  $airing = entity_create('airing', array());

  $start_time =
    strtotime($row['Schedule Item Log Date'] . ' ' . $row['Schedule Item Log Time']);

  $end_time = $start_time +
    strtotime("1970-01-01 " . $row['Schedule Item Duration']." UTC");
    
  $airing->field_airing_date[LANGUAGE_NONE][0]['value'] =
    date('Y-m-d H:i:s', $start_time);

  $airing->field_airing_date[LANGUAGE_NONE][0]['value2'] =
    date('Y-m-d H:i:s', $end_time);

  $airing->field_airing_project_ref[LANGUAGE_NONE][0]['target_id']=$row['project_nid'];
  $airing->field_airing_show_ref[LANGUAGE_NONE][0]['target_id'] = $row['show_nid'];
  $airing->field_airing_channel[LANGUAGE_NONE][0]['tid']  = 1686;
  $airing->field_airing_type[LANGUAGE_NONE][0]['tid'] = 969;
  $airing->cm_use_auto_title = TRUE;
  entity_save('airing', $airing);
  $airing->airing_id = entity_id('airing', $airing);
  return $airing;
}

////////////////////////////////////////////////
//ERIC'S OLD CODE

/**
structure of airing nodes 

'field_airing_date',

field_airing_date (Array, 1 element)

    und (Array, 1 element)
        0 (Array, 5 elements)
            value (String, 19 characters ) 2017-02-23 18:31:01
            value2 (String, 19 characters ) 2017-02-23 18:34:33
            timezone (String, 3 characters ) UTC
            timezone_db (String, 3 characters ) UTC
            date_type (String, 8 characters ) datetime




 'field_airing_channel',
 
  field_airing_channel (Array, 1 element)

    und (Array, 1 element)
        0 (Array, 2 elements)
            tid (String, 3 characters ) 965
            taxonomy_term (Object) stdClass 
 
         'field_airing_project_ref', 
 
field_airing_show_ref (Array, 1 element)

    und (Array, 1 element)
        0 (Array, 3 elements)
            target_id (String, 6 characters ) 888276
            entity (Object) stdClass
            access (Boolean) TRUE

        
         
         'field_airing_show_ref',

field_airing_project_ref (Array, 1 element)

    und (Array, 1 element)
        0 (Array, 3 elements)
            target_id (String, 6 characters ) 676700
            entity (Object) stdClass
            access (Boolean) TRUE



         'field_airing_type'
         
          field_airing_type (Array, 1 element)

    und (Array, 1 element)
        0 (Array, 2 elements)
            tid (String, 3 characters ) 970
            taxonomy_term (Object) stdClass 


Creating new entities is as easy as:

  $entity_type = 'contact';
  $entity = entity_create($entity_type, array('type' => 'contact_simple'));
  $wrapper = entity_metadata_wrapper($entity_type, $entity);

Once the entity is created the properties can be changed:

$wrapper->uid = 9;

And any attached field:

$wrapper->field_full_name->set("John doe");

Alternatively, field values can be set directly as the EntityMetadataWrapper class (provided by EntityAPI) implements PHP's magic __set() method. For example:

$wrapper->field_full_name = "John doe";

Just remember to save it when all changes are made:

$wrapper->save();

That's it!



*/
/**
  global $user;

  $entity_type = 'airing';
  $airing = entity_create('airing', array('type' => 'airing'));
  $wrapper = entity_metadata_wrapper($entity_type, $airing);
//print_r($wrapper);
//  $wrapper->uid->set($user->uid);
//  $wrapper->uid = "6";
//  $wrapper->field_airing_date->set("2017-02-23 18:31:01");

$start_timestamp = strtotime("2017-02-23 18:31:01");
$end_timestamp = strtotime("2017-02-23 18:31:01");

//  $wrapper->field_airing_date->set(array(
    // Should be in unix timestamp format.
//    'value' => $start_timestamp,
//    'value2' => $end_timestamp,
//  ));
 
  $wrapper->field_airing_title->set("test title");
  $wrapper->field_airing_channel->set("965");
  $wrapper->field_airing_show_ref->set("888276");
  $wrapper->field_airing_project_ref->set("676700");
  $wrapper->field_airing_type->set("970");
  $thing = $wrapper->save();

$fp = fopen('/var/www/cm-stage.mnn.org/sites/all/modules/lidn.txt', 'w');
fwrite($fp, print_r($thing, TRUE));
fclose($fp);
*/
