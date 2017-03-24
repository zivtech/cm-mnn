<?php

function cma_program_import_airing_create($data) {

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

*/

  global $user;

  $node = new stdClass();
  $node->type = "cm_show";
  node_object_prepare($node);
  
  $node->title    = "";
  $node->uid = $user->uid;
  $node->language = LANGUAGE_NONE;



  //FIXME ONLY HERE FOR TESTING
  $airing = new stdClass();
  $airing->airing_id = 16945;
  return $airing;
}