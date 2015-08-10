<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */
 
 
function cm_airing_grid_grid_header_form ($form, &$form_state, $channel, $channels, $channel_tid, $start_time, $end_time, $mysql_day_of_week, $collapsed = false, $grid_mode = "filter_end") {
	
  global $base_url;
  // not sure what the intent was for the $form_header, but setting it
  // to NULL to address issue #1732708
  $form_header = NULL;

  $form['#action'] = url($base_url.'/admin/manage/scheduling');
  	
  $form['collapsed_grid_header'] = array(
    '#title' => "<b>".t("Navigation")."</b>",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed 
  ); 
  $form['collapsed_grid_header']['grid_header'] = 
    array('#value' => $form_header,   
        '#prefix' => '<table border=1 class="cm_airing_grid_bordered"><tr valign="center">' .
            '<td class="cm_airing_grid_bordered">',
   );
 
  cm_airing_grid_get_date_options($years, $months, $days,
    $hours, $minutes, $ampm);

  $spacer = '<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';    
  $form['cm_airing_grid_grid_hour'] = array(
    '#type' => 'select',
    '#title' => t('Hour'),
    '#default_value' => date('h', $start_time),
    '#options' => $months,
    '#prefix' => '<table class="cm_airing_grid_grid_header"><tr valign="center">' .
        "$spacer<td>",
    '#suffix' => '</td>'.$spacer,
  );
  $form['cm_airing_grid_grid_ampm'] = array(
    '#type' => 'select',
    '#title' => t('am/pm'),
    '#default_value' => date('a', $start_time),
    '#options' => $ampm,    
    '#prefix' => '<td>',
    '#suffix' => '</td>'.$spacer,
  );
  $form['cm_airing_grid_grid_month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#default_value' => date('m', $start_time),
    '#options' => $months,    '#prefix' => '<td>',
    '#suffix' => "</td>$spacer",
  );
  $form['cm_airing_grid_grid_day'] = array(
    '#type' => 'select',
    '#title' => t('Day'),
    '#default_value' => date('d', $start_time),
    '#options' => $days,
    '#prefix' => '<td>',
    '#suffix' => "</td>$spacer",
  );
  $form['cm_airing_grid_grid_year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#default_value' => date('Y', $start_time),
    '#options' => $years,
    '#prefix' => '<td>',
    '#suffix' => "</td>$spacer",
  );
  $form['cm_airing_grid_channel_select'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#default_value' => $channel_tid,
    '#options' => $channels,
    '#prefix' => '<td>',
    '#suffix' => "</td>$spacer",
  );
  $form['cm_airing_grid_grid_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#default_value' => $grid_mode,
    '#options' => array("filter_end"=>"Series Mode", 
      "show_all"=>"Fill Mode/Show All"),
    '#prefix' => '<td>',
    '#suffix' => "</td>$spacer</tr></table>",
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#prefix' => '<table class="cm_airing_grid_grid_header"><tr><td>',
    '#suffix' => '</td></tr></table><br/></td></tr></table><br/><br/>',
    );
  
  // $output .= drupal_render(drupal_get_form('cm_airing_grid_grid_form', $form));

  return $form;   
} 

function cm_airing_grid_get_grid_header
($channel, $start_time, $end_time, $mysql_day_of_week, $collapsed = false,
		$grid_mode = "filter_end") {
	global $base_url;

	$channels = cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());
	$channel_number = preg_replace("/Channel /", "", $channel);
	$channel_tid = array_search($channel, $channels);
	
	$form_header = "<h1>$channel Grid</h1> ";
	$form_header .= "<h1>".date('l F j, Y', $start_time)."</h1>";
	
	$data = array();
	$rows = array();
	$yesterday = $start_time - (60*60*24);
	$tomorrow = $start_time + (60*60*24);
	
	$data[] = l("<<<Yesterday", $base_url."/admin/manage/scheduling/$channel_number/" .
			"$yesterday/".date('H',$start_time)."//$grid_mode");
	$data[] = "&nbsp;&nbsp;";
	$data[] = "<h1>".date("ga", $start_time). " to ".
			date("ga", ($end_time + 1))."</h1>";
	$data[] = "&nbsp;&nbsp;";
	$data[] = l("Tomorrow>>>", $base_url."/admin/manage/scheduling/$channel_number/" .
			"$tomorrow/".date('H',$start_time)."//$grid_mode");
	
	cm_airing_grid_format_status_row($rows, $data, "cm_airing_grid_grid_header", "clear");
	
	$form_header .=
	theme('table', array('rows' => $rows, 'attributes' => array('class' => 'cm_airing_grid_grid_header')));

	$form_header .= drupal_render(drupal_get_form('cm_airing_grid_grid_header_form', $channel, $channels, $channel_tid, $start_time, $end_time, $mysql_day_of_week, $collapsed,
		$grid_mode));
	
	return $form_header;
		 
}


function cm_airing_grid_grid_header_form_submit($form, &$form_state) {
  $channels = cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());
  $hour = _class_scheduler_fix_hour($form_state['values']['cm_airing_grid_grid_hour'], 
      $form_state['values']['cm_airing_grid_grid_ampm']);  
  $new_time = mktime($hour, 0, 0, 
     $form_state['values']['cm_airing_grid_grid_month'],
      $form_state['values']['cm_airing_grid_grid_day'], 
      $form_state['values']['cm_airing_grid_grid_year']);
  $channel_tid = $form_state['values']['cm_airing_grid_channel_select'];
  $channel = $channels[$channel_tid];
  $channel = preg_replace("/Channel /", "", $channel);
  $mode = $form_state['values']['cm_airing_grid_grid_mode'];
  drupal_goto ("admin/manage/scheduling/$channel/$new_time/$hour//$mode");
  
}
 


function cm_airing_grid_airing_detail_form($airing_nid)  {
  global $base_url;
  $airing = node_load($airing_nid);
  $output = node_view($airing);
  if ( is_array($airing->parent_node) ) {
    $parent_node = node_load($airing->parent_node[0]);
    if ( count($airing->parent_node) > 1 ) {
      drupal_set_message('warning: node '.$airing->nid.' has multiple parents', 
        'error');
    }
  }
  else if ( $airing->parent_node) { 
    $parent_node = node_load($airing->parent_node);
  }
  /* 
   * FIXME: oh the hardcoded horror
   */
  if ( $parent_node->type == 'content_episode' ) {
    $episode = $parent_node;
    if ( $episode->parent_node ) {
      $proj = node_load($episode->parent_node);
      $proj_nid = $proj->nid;
    }
  }
  else if ( $parent_node->type == 'content_project' ) {
    $proj = $parent_node;
    $proj_nid = $proj->nid;
  }
  $ep_option_list[] = "<none>";
  $ep_list_res = cm_airing_grid_get_all_non_live_episode_nids_for_project($proj_nid);
  while ( $ep = db_fetch_array($ep_list_res) ) {
    $ep_option_list[$ep['nid']] = 'Episode #' . $ep['field_episode_number_value'];
  }

  $edit_url = url("$base_url/node/$airing_nid/edit");
  $full_destination = drupal_get_destination();
  $destination = cm_airing_grid_remove_last_url_element(urldecode($full_destination));
  $delete_url = url("$base_url/node/$airing_nid/delete") ."?". $destination;
  
  $project_format = cm_airing_grid_get_node_format($proj);
  
  if ($episode) {
    $episode_format = cm_airing_grid_get_node_format($episode);
  }
  if ( cm_airing_grid_is_format_live($project_format['name']) ||
       cm_airing_grid_is_format_live($episode_format['name']) ){
    $form['header'] = array( 
      '#value' => '<h3>Live Airing Info</h3>', 
      '#weight' => -16,  //taxonomy has weight -15 (!)
    );    
    if ( $episode->taxonomy ) {
      $dvid = cm_airing_grid_get_device_vid();
      $ep_device_array = cm_airing_grid_extract_tids_by_vid($dvid, $episode);
      $ep_device_tid = $ep_device_array[0]->tid;
    }
    cm_airing_grid_airing_form_live_episode_dropdowns
      ($form, $airing_nid, $episode, $proj_nid, $ep_device_tid, $ep_option_list);
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Set Device/Episode'),
    );
    $detail_select .= drupal_get_form('cm_airing_grid_grid_airing_detail_device_form', $form);
  }
  else {
    $form['header'] = array( 
      '#value' => '<h3>Episode For This Airing:</h3>',
      '#weight' => -10      
     );
    cm_airing_grid_airing_form_episode_dropdown
      ($form, $airing_nid, $episode, $proj_nid, $ep_option_list);
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Set Episode'),
    );
    /*
     * list of episodes
     */

    $detail_select .= drupal_get_form('cm_airing_grid_grid_airing_detail_ep_select_form', $form);
  }
  $output = ereg_replace('<div class="links">.*calendar</a></div>', "", $output);
  if ($episode) {
    $output .= "Project: ".l($proj->title, "node/$proj->nid")."<br/><br/><br/>";
  }
  $output .=
    l(t('Edit'), $edit_url)
    ."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".
    l(t('Delete'), $delete_url);
  $delete_future_url = implode('/', array(
                                      'programming/project_cancel',
                                      $proj->nid,
                                      $airing_nid));
  $output .= "<br />";
  $output .= "<br />";
  $output .=
    l(t('Delete this and all future Airings for this Project'),
      $delete_future_url);

  $output = '<table border=1 class="cm_airing_grid_bordered">' .
      '<tr valign="center"><td class="cm_airing_grid_bordered">'.
      "<h1>Project ID: ".$proj->field_project_id[0]['value']."<br/>MSN: ".$proj->field_msn[0]['value'].$output.'</td></tr>'.
      '<tr><td class="cm_airing_grid_bordered">'.$detail_select.'</td></tr>'.
      '</table><br/><br/>';

  return $output;
}
function cm_airing_grid_grid_airing_detail_ep_select_form_validate
  ($form_id, &$form_values){
  if ($form_values['episode']) {
    $airing_nid = $form_values['airing_nid'];
    $airing = node_load($airing_nid);
    $channel_info = cm_airing_grid_what_channel($airing);
    $new_episode_nid = $form_values['episode'];
    $new_ep_length = cm_airing_grid_get_episode_length($new_episode_nid);
    if (!$new_ep_length) {
      return;
    }
    $airing->event_end = $airing->event_start + $new_ep_length; 
    $next_airing_res = cm_airing_grid_check_for_overlap_by_node
      ($airing, $channel_info['tid']);
    $next_airing = $next_airing_res[0];
    if ($next_airing) {
      form_set_error('episode', 
        t("Sorry, the episode you selected has a runtime of $new_ep_length ".
          "seconds (".($new_ep_length/60).
          " minutes) which overlaps with the next Airing, ".
            l($next_airing->title, "node/$next_airing->nid").
            ". Either edit/delete that ".
            l('Airing', "node/$next_airing->nid/edit").
            " or select another Episode. You could also ".
            l('edit the timecode fields', "/node/$new_episode_nid/edit").
            " on the Episode."));
    }
  
  }  
}
function cm_airing_grid_grid_airing_detail_device_form_validate($form_id, $form_values){
  if ( $form_values['episode'] && $form_values['device'] ) {
    form_set_error('device', t('Please select only one of either a device or episode'));
  }
}

function cm_airing_grid_grid_airing_detail_ep_select_form_submit($form_id, $form_values){
  return cm_airing_grid_grid_airing_detail_device_form_submit($form_id, $form_values);
}

function cm_airing_grid_grid_airing_detail_device_form_submit($form_id, $form_values) {
  $airing_nid = $form_values['airing_nid'];
  $airing = node_load($airing_nid);
  $resave_airing = false;
  $project_nid = $form_values['project_nid'];
  $old_episode_nid = $form_values['old_episode_nid'];
  $device_tid = $form_values['device'];
  $new_episode_nid = $form_values['episode'];
  cm_airing_grid_airing_form_episode_dropdown_submit($airing, $project_nid,
    $old_episode_nid, $device_tid, $new_episode_nid);
  
  $new_ep_length = cm_airing_grid_get_episode_length($new_episode_nid);
  $new_airing_title = cm_airing_grid_airing_auto_node_title($airing);
  if ($new_ep_length) {
    $airing->event_end = $airing->event_start + $new_ep_length;
    $resave_airing = true;
  }
  if ($new_airing_title != $airing->title ) {
    $airing->title = cm_airing_grid_airing_auto_node_title($airing);
    $resave_airing = true;
  }
  if ($resave_airing) {
    node_save($airing);
  }
}


function cm_airing_grid_airing_form_live_episode_dropdowns
      (&$form, $airing_nid, $episode, $project_nid, $ep_device_tid, $ep_option_list) {
  $dvid = cm_airing_grid_get_device_vid();     
  if ( $episode && $episode->nid ) {
    // "old" meaning before we hit submit on this form
    $form['old_episode_nid'] = array(
          '#type' => 'value',
          '#value' => $episode->nid,
     );
  }
  $form['project_nid'] = array(
        '#type' => 'value',
        '#value' => $project_nid,
  );
  $form['airing_nid'] = array(
        '#type' => 'value',
        '#value' => $airing_nid,
  );
  $form['device'] = taxonomy_form($dvid, $ep_device_tid ? $ep_device_tid : NULL );
  $form['device']['#title'] = t("Device for Live Airing");
  $description = t("Select a Device for the Live Project's Airing.");
  if (!$episode) {
    $description .= "<br/>".
      t("Selecting and submitting will create a Llve Episode for the Project."); 
  } 
  $form['device']['#description'] = $description;
  $form['device']['#weight'] = -4.6;
   
  $form['episode'] = array(
        '#type' => 'select',
        '#title' => t('Replacement Episode'),
        '#default_value' => ($episode) ? $episode->nid : NULL,
        '#options' => $ep_option_list,
        '#weight' => -4.5,
        '#description' => 
          t("If the Project's Airing is no longer going to be live, ".
            "select a replacement Episode."),
  );
}


function cm_airing_grid_airing_form_episode_dropdown
  (&$form, $airing_nid, &$episode, $project_nid, &$ep_option_list) {
    if ($episode &&  $episode->nid ) {
    // "old" meaning before we hit submit on this form
    $form['old_episode_nid'] = array(
          '#type' => 'value',
          '#value' => $episode->nid,
     );
  }
  $form['project_nid'] = array(
        '#type' => 'value',
        '#value' => $project_nid,
  );
  $form['airing_nid'] = array(
        '#type' => 'value',
        '#value' => $airing_nid,
  );
  
  $form['episode'] = array(
      '#type' => 'select',
      '#title' => ($episode) ? t('Change Episode') : t('Select Episode'),
      '#options' => $ep_option_list,
      '#default_value' => ($episode) ? $episode->nid : NULL,
      '#weight' => -4.7,
    );   
}


/**
 *  TODO: This function can be simplified.
 */
function cm_airing_grid_airing_form_episode_dropdown_submit(&$airing, $proj_nid,
  $old_ep_nid, $device_tid, $new_ep_nid) {
  $proj = node_load($proj_nid);
  if ( $old_ep_nid ) {
    $old_ep = node_load($old_ep_nid);
  }
  else {
    $old_ep = NULL;
  }
  $new_ep = NULL;
  $del_ep_nid = NULL;
  if ( $device_tid && $new_ep_nid ) {
    drupal_set_message('an attempt to set both a device and pre-recorded episode was made. aborting.', 'error');
  }
  //project or the existing episode is live
  else if ( cm_airing_grid_is_node_live_format($proj) ||
             cm_airing_grid_is_node_live_format($old_ep)) { 
    if ( $old_ep ) { // changing an existing setting
      if ( cm_airing_grid_is_node_live_format($old_ep) ) {
        $old_ep_device = cm_airing_grid_get_device_info($old_ep);
        if ( $device_tid ) {  //user selected a device
          if ( $device_tid != $old_ep_device['tid'] ) {
            $dvid = cm_airing_grid_get_device_vid();
            cm_airing_grid_flip_taxonomy($device_tid, $dvid, $old_ep);
          }
          else {
            //nothing is changing at all
          }
        }
        else if ( $new_ep_nid ) {  //user selected an episode
          $del_ep_nid = $old_ep->nid;
          $old_ep = NULL;
          $new_ep = node_load($new_ep_nid);
        }
        else {
          $del_ep_nid = $old_ep->nid;
        }
      }
      else {  // old episode was taped
        if ( $device_tid ) {
          $new_ep = cm_airing_grid_create_live_episode_from_form_submit($proj, 
                      $device_tid, $airing);
        }
        else if ( $new_ep_nid ) {
          if ( $new_ep_nid != $old_ep->nid ) {
            $new_ep = node_load($new_ep_nid);
          }
          else {
            //nothing is changing at all
          }
        }
        else {
          cm_airing_grid_unlink_relativity_relationship($airing->nid, $old_ep->nid);
          cm_airing_grid_insert_relativity_relationship($airing->nid, $proj_nid);
        }
      }
    }
    else { // no episode is set, so set one
      if ( $new_ep_nid ) {
        $new_ep = node_load($new_ep_nid);
      }
      else if ( $device_tid ) {
        $new_ep = 
          cm_airing_grid_create_live_episode_from_form_submit($proj, $device_tid, $airing);
      }
      else {
        // nothing was selected, so do nothing
      }
    }
  }
  else { // project is not live
    if ( !$new_ep_nid ) {
      cm_airing_grid_unlink_relativity_relationship($airing->nid, $old_ep->nid);
      cm_airing_grid_insert_relativity_relationship($airing->nid, $proj_nid);
    }
    else {
      $new_ep = node_load($new_ep_nid);
    }
  }
  if ( $del_ep_nid ) {
    node_delete($del_ep_nid);
  }
  if ( $old_ep && $new_ep ) {
    cm_airing_grid_unlink_relativity_relationship($airing->nid, $old_ep->nid);
    cm_airing_grid_insert_relativity_relationship($airing->nid, $new_ep->nid);
    cm_airing_grid_insert_relativity_relationship($new_ep->nid, $proj_nid);
  }
  else if ( $new_ep ) {
    cm_airing_grid_unlink_relativity_relationship($airing->nid, $proj_nid);
    cm_airing_grid_insert_relativity_relationship($airing->nid, $new_ep->nid);
    cm_airing_grid_insert_relativity_relationship($new_ep->nid, $proj_nid);
  }
  $airing->episode = $new_ep;
}
