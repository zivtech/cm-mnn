<?php

$query = new EntityFieldQuery();

$query->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', 'cm_project')
  ->propertyCondition('status', 1);

$result = $query->execute();
$loop_limiter = 0;

if (!empty($result['node'])) {
      civicrm_initialize();

// load only the nid's to avoid memory issues 
  $nids = array_keys($result['node']);

//loop over nids, load node, update field, save 
 foreach ($nids as $nid) {
    if($loop_limiter <= 1){
      $node = node_load($nid, NULL, TRUE);
print_r($node->nid." \n");
      $loop_limiter = $loop_limiter +1;

print_r($node->field_project_public_email);
$project_public_email = $node->field_project_public_email['und'][0]['value'];
//     $project_public_email = cm_crew_connect_get_single_field_value($node,
//					     'field_project_public_email ',
//					     'value');
		 print_r($project_public_email." \n");
		 if(isset($project_public_email)){
		     $producer_cid = CRM_Core_BAO_UFMatch::getContactID($node->uid);
print_r($producer_cid." \n");

              $result_save_email = civicrm_api3('Email', 'create', array(
              'sequential' => 1,
              'contact_id' => $producer_cid,
              'email' => $project_public_email,
               'location_type_id' => 6,
               ));
      
  }
 }     
 }
 }