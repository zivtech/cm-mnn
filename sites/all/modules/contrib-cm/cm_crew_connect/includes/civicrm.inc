<?

function cm_crew_connect_get_display_name($uid) {
  civicrm_initialize();
  $result = civicrm_api3('UFMatch', 'get', array(
						 'sequential' => 1,
						 'uf_id' => $uid,
						 ));

  $cid = (isset($result) && isset($result['values']) && 
	  isset($result['values'][0])) ? $result['values'][0]['contact_id'] :
    NULL;

  if ($cid) {
    $result = civicrm_api3('Contact', 'get', array(
						   'sequential' => 1,
						   'id' => $cid,
						   ));

    $display_name = (isset($result) && isset($result['values']) && 
		     isset($result['values'][0])) ? 
      $result['values'][0]['display_name'] : NULL;
    return $display_name;
  }
  return "";
}