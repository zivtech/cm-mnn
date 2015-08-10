<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */



function cm_airing_grid_extract_grid_data($airings_grid_array, $column_time, 
  $start_hour, $start_time, $channel, $grid_mode) {
  global $base_url;
  $channel_number = preg_replace("/Channel /", "", $channel);
  $data = isset($airings_grid_array[$column_time]) ? $airings_grid_array[$column_time] : NULL;
  $count = 1;
  if ($data) {

    $text = "<table class = 'programming_internal_grid'>";

    foreach($data as $airing) {

      if ($airing->event_start == date("Y-m-d H:i:s", $column_time)) { 
        $display_class = cm_airing_grid_grid_display_class_dictionary($airing);
        $link = l($airing->project_nid, "airing/".$airing->airing_nid."/view");
        if ($grid_mode == 'show_all') {
          $link .= "<br/>".l("(replace)", "airing/".$airing->airing_nid."/edit");
        }
        $text .= "<tr><td class='$display_class'>".$link.
            "</td></tr>";
        $display_class = 'programming_grid_blank';
        if ($airing->count > $count) {
          $count = $airing->count;
        }
      }
    }
    if ($grid_mode == 'show_all') {
        $url = url($base_url.'/airing/add/'.
            (strtotime($airing->event_end)+1).'/'.$channel_number.'/at-fill') ;
        $add_link = '<a class="programming-grid" href="'.$url.'">'.
            "+".'</a>';
        $text .= "<tr><td class='programming_grid_borderless'>".$add_link.
          "</td></tr>";
    }
    $text .= "</table>";
        
    return array('text'=>$text, "colspan"=>$count, 'grid_data_class' => $display_class);
  }
  return null;         
}

function cm_airing_grid_format_status_row
    (&$rows, $data, $data_classes = null, $row_class = null) {
  if (!$data_classes){
    $data_classes = array('replication', 'replication');
  }
  if (!$row_class) {
    $row_class = "replication_grey";
  }
  if (count($data) == 1) {
    $cells[] = array('data' => $data[0],
                     'class' => array($data_classes[0]),
                     'colspan' => 2,
                     );
    $rows[] = array('data' => $cells, 'class' => $row_class);
  }
  else if (count($data) == 2) {
    $cells[] = array('data' => $data[0],
                     'class' => array($data_classes[0]),
                    );
    $cells[] = array('data' => $data[1],
                     'class' => array($data_classes[1]),
                    );
    $rows[] = array('data' => $cells, 'class' => array($row_class));
  }
  else if (count($data) > 2) {
    foreach ($data as $key=>$value) {
      if (is_array($data_classes)) {
        $data_class = $data_classes[$key];
      }
      else {
        $data_class = $data_classes;
      }
      if (is_array($value)) {
        $colspan = $value[1];
        $value = $value[0];
      }
      else {
        $colspan = 1;
      }
      $cells[] = array('data' => $value,
                        'class' => array($data_class),
                        'colspan' => $colspan,
                       );
      
    }
    $rows[] = array('data' => $cells, 'class' => array($row_class));
  }
}


/*
 * try to remove the last element from a url
 */
function cm_airing_grid_remove_last_url_element($url) {
  $a = explode("/", $url);
  if ( $unused = array_pop($a) ) {
    return implode("/", $a);
  }
  else {
    return $url;
  }
}


function cm_airing_grid_airing_auto_node_title(&$node){
  $project_nid = NULL;
  $title = NULL;
  $episode_number = NULL;
  if ( $title = $node->programming_project_name ) {
    //nothing
  }
  else if ( $project_nid = $node->programming_project_nid ) {
    $proj = node_load($project_nid);
    if ( $proj->title ) {
      $title = $proj->title;
    }
  }
  else {
    if ( $node->nid ) {
      $project_nid = programming_get_project_nid_for_airing($node->nid); 
    }
    $proj = node_load($project_nid);
    if ( $proj ) {
      $title = $proj->title;
    }
    else {
      return "[ * Airing Without A Project, Please Fix Me]";
    }
  }
  if ( $episode_number = $node->programming_episode_number ) {
    $title .= " Ep-$episode_number";
  }
  else if ($node->nid) {
    $episode_nid = programming_get_episode_nid_for_airing($node->nid);
    if ($episode_nid && is_numeric($episode_nid)) {
      $episode = node_load($episode_nid);
      $title .= " Ep-".$episode->field_episode_number[0]['value'];
    }
  }
  $channel_info = programming_what_channel($node);
  if ($channel_info['name']) {
    $title .= preg_replace("/Channel /", " Ch-", $channel_info['name']);
  }
  return $title." ".date('m/d/Y', $node->event_start);
}


function cm_airing_grid_create_live_episode_from_form_submit
  (&$project, $device_tid, &$airing) {
  $dvid = programming_get_device_vid();
  $new_ep_num = db_result(programming_get_max_episode_number_for_project($project->nid));
  $new_ep->field_episode_number[0]['value'] = $new_ep_num + 1;
  $new_ep->programming_project_nid = $project->nid;
  $new_ep->programming_project_name = $project->title; //used by nodeapi for titling
  $new_ep->created = time();
  $new_ep->changed = time();
  $new_ep->comment = 0;
  $new_ep->promote = 0;
  $new_ep->uid = 0; // TODO: what uid?
  //FIXME: hardcoded type again
  $new_ep->type = 'content_episode';
  node_save($new_ep);
  
  $new_ep_taxonomy = array(programming_get_device_vid() => $device_tid);
  
  //set FORMAT = LIVE
  $new_ep_taxonomy[programming_get_format_vid()] = 
    programming_get_live_format_tid();
  
  //set Schedule Type based on Airing type
  $airing_type = programming_get_airing_type($airing);
  //We call this method because an Airing can be a 'Series Makeup', but 
  //an Episode is still considered a 'Series', even if the Episode was
  //aired as a Makeup
  $schedule_type = programming_airing_type_to_schedule_type($airing_type, $project);
  $new_ep_taxonomy[programming_get_schedule_type_vid()] = $schedule_type['tid'];
  
  //set Runtime
  $runtime_tid = programming_get_runtime_for_new_episode($airing, $project);
  $new_ep_taxonomy[programming_get_runtime_vid()] = $runtime_tid;
  taxonomy_node_save($new_ep->nid, $new_ep_taxonomy);
  $new_ep = node_load($new_ep->nid);
  
  return $new_ep;
}
