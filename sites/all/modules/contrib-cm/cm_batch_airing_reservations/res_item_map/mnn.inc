<?php
/**
 * @file
 * This file is custom to MNN, for other stations make a new file in this directory
 * and change the file name for the module settings.
 */


function _cm_batch_airing_reservations_res_item_map() {
  $items = array();
  $items =
    array(  
	  840 => array(), 
	  849 => array(
		       'item_id' => 374872,
		       'type' => '59th_express_studios_fac_',
		       'inventory_tid' => 1680,
		       'start_offset' => 30,
		       'end_offset' => 2, 
		       ), 
	  848 => array(
		       'item_id' => 374871 ,
		       'type' => '59th_express_studios_fac_',
		       'inventory_tid' => 1680,
		       'start_offset' => 30,
		       'end_offset' => 2, 
		       ), 
	  847 => array(		       
		       'item_id' => 374850,
		       'type' => '59th_open_closed_studios_fac_',
		       'inventory_tid' => 1680,
		       'start_offset' => 60,
		       'end_offset' => 2, 
				       ), 
	  846 => array(
		       'item_id' => 374849,
		       'type' => '59th_open_closed_studios_fac_',
		       'inventory_tid' => 1680,
		       'start_offset' => 60,
		       'end_offset' => 2, 
		       ), 
	  843 => array(),
	  915 => array(), 
	  845 => array(), 
	  844 => array(), 
	  1144 => array(), 

	  ); 
  return $items;
}
