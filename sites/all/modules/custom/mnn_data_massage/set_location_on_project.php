<?php

// get a list of all nodes of type project 
$query = new EntityFieldQuery();

$query->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', 'cm_project')
  ->propertyCondition('status', 1);

$result = $query->execute();
$loop_limiter = 0;

if (!empty($result['node'])) {

// load only the nid's to avoid memory issues 
  $nids = array_keys($result['node']);

//loop over nids, load node, update field, save 
 foreach ($nids as $nid) {
    if($loop_limiter <= 1000000){
      $node = node_load($nid, NULL, TRUE);
      $loop_limiter = $loop_limiter +1;
//     print_r($node->field_location['und'][0]['tid']);
//get the locaiton tid from the node 
     $locationtid = cm_crew_connect_get_single_field_value($node,
					     'field_location',
					     'tid');
		 print_r($locationtid);
		 if(!isset($locationtid)){
		   print_r($node-nid);
		   $node->field_location['und'][0]['tid'] = 834;
		   $done = node_save($node);
		 }			     
    }
  }
}  