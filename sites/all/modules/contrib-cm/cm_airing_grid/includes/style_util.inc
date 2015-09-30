<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */


function cm_airing_grid_grid_display_class_dictionary(&$airing) {
  $format_vid = cm_airing_grid_get_format_vid();
  if ($airing->episode_nid) {
    $format_tid = cm_airing_grid_get_project_format_tid($airing->episode_nid);
    if ($format_tid) { 
      $format_term_name = cm_airing_grid_get_term_name_for_tid($format_tid);
    }
  }
  else {
    $format_tid = cm_airing_grid_get_project_format_tid($airing->project_nid);
    if ($format_tid) { 
      $format_term_name = cm_airing_grid_get_term_name_for_tid($format_tid);
    }

  }
  if ( $format_term_name == "LIVE" ) {
    if ( cm_airing_grid_get_episode_device_tid($airing->episode_nid) ) {
      return 'cm_airing_grid_live';
    }
    else {
      return 'cm_airing_grid_no_device';
    }
  }
  else if ((strtotime($airing->event_end) - 
	    strtotime($airing->event_start)) < 20*60) {
    return 'cm_airing_grid_short';
  }
  else {
    if ( $airing->episode_nid ) {
      return 'cm_airing_grid_data';
    }
    else {
      return 'cm_airing_grid_no_episode';
    }
  }
}
