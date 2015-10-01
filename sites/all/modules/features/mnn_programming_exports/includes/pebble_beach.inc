<?php

function mnn_programming_exports_pebble_beach() {
  //fetch our view
  $view = views_get_view(MNN_DAILY_SHOWS_VIEW);

  //execute the query and grab the results
  $view->execute();
  $results = $view->result;
  if (isset($view->exposed_input['field_airing_date_value'])) {
    $date = ($view->exposed_input['field_airing_date_value']) ?
      strtotime($view->exposed_input
		['field_airing_date_value']['value']['date'])
      : strtotime('today');
    $channels = mnn_programming_exports_pebble_beach_content($results,$date);
    foreach ($channels as $channel=>$result) {
      $output .= l("Download Pebble Beach File For Channel #$channel",
		   "admin/shows/pebble-beach/download/$channel")."<br/>";
    }
  }
  $old_action = 'action="/'.MNN_DAILY_SHOWS_VIEW_PATH_RAW;
  $new_action = 'action="/'.MNN_PEBBLE_BEACH_VIEW_PATH;

  $output .= mnn_programming_exports_widgets($view,$old_action,$new_action);
  
  return $output;
}

function mnn_programming_exports_pebble_beach_content($results,$date_filter) {
  $content = array();
  foreach($results as $result) {
    if (!isset($result->field_field_airing_project_ref[0])) {
      drupal_set_message("Missing project for airing: ".
			 l($result->airing_id, "airing/".$result->airing_id), 
			 "error");
      continue;
    }
    $project = $result->field_field_airing_project_ref[0]['raw']['entity'];

    if (!isset($result->field_field_airing_date[0])) {
      drupal_set_message("Missing Date for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $date = $result->field_field_airing_date[0]['raw'];
    
    if (!isset($result->field_field_airing_channel[0])) {
      drupal_set_message("Missing Channel for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $channel = $result->field_field_airing_channel[0]['rendered']['#title'];

    if (!isset($project->taxonomy_cm_project_types[LANGUAGE_NONE])) {
      drupal_set_message("Missing Project Type for Project:".
			 l($project->nid,"node/".$project->nid),
			 "error");
      continue;
    }
    $show = null;
    if (isset($result->field_field_airing_show_ref[0])) {
      $show = $result->field_field_airing_show_ref[0]['raw']['entity'];
    }

    if (!$show) {
      continue;
    }

    if (!isset($show->field_format[LANGUAGE_NONE])) {
      drupal_set_message("Missing Format for Show:".
			 l($show->nid,"node/".$show->nid),
			 "error");
      continue;
    }
    $format_tid = $show->field_format[LANGUAGE_NONE][0]['tid'];
    $format_name = taxonomy_term_load($format_tid);
    if(!$format_name) {
      drupal_set_message("Missing Format for tid :".$format_tid,"error");
      continue;
    }
    $format_name = $format_name->name;

    $livefeed_tid = 
      isset($show->field_device[LANGUAGE_NONE][0]) &&
      isset($show->field_device[LANGUAGE_NONE][0]) ?
      $show->field_device[LANGUAGE_NONE][0]['tid'] : NULL;
    if ($livefeed_tid) {
      $livefeed = taxonomy_term_load($livefeed_tid);
      if ($livefeed) {
	$livefeed_name = $livefeed->name;
      }
    
    }
    
    $data['project_id'] = $project->nid;
    $data['end'] = strtotime($date['value2']);
    $data['start'] = strtotime($date['value']);
    $data['channel'] = str_replace('Channel ', '',$channel);
    $data['episode'] = 
      $show->field_episode_number[LANGUAGE_NONE][0]['value'];
    $data['house_id'] = $data['project_id']."_".$data['episode'];
    //$data['title'] = str_replace('&', '&amp', $project->title);
    $data['title'] = str_replace('&', '', $project->title);
    $data['format'] = $format_name;
    $data['runtime'] = $data['end'] - $data['start'];
    $data['livefeed'] = $livefeed_name;
    $data['duration'] = gmdate('H:i:s', $data['end'] - $data['start']).
      ";02"; 
    $data['start_offset'] = date('Y-m-d',$data['start'])."T".
      date('H:i:s',$data['start']).";00";

    $content[$data['channel']] = isset($content[$data['channel']]) ?
      $content[$data['channel']] : "";

    $content[$data['channel']] .=  
      mnn_programming_exports_pebble_beach_xml($data);
  }
  
  $_SESSION['mnn_pebble_beach_content'] = $content;
  $_SESSION['mnn_pebble_beach_date'] = $date_filter;

  return $content;
}

function mnn_programming_exports_pebble_beach_xml($data) {
  $time = 
    '<schedule endType="Duration" endOffset="'.$data['duration'].
    '" startType="Fixed" startOffset="'.$data['start_offset'].
    '"/>';
  
  $house_id_info = 
    '
     <event title="'.$data['title'].'" houseId="'.
     $data['house_id'].'" validationMode="Update"/>
    ';
  
  if (strtolower($data['format']) == 'live') {
    $livefeed_array = mnn_programming_exports_livefeed_array($data['channel']);
    $fixed_port = $livefeed_array[$data['livefeed']];

    if (!$fixed_port) {
      drupal_set_message("ERROR! missing port. error code:4347384". 
			 $livefeed_array['livefeed'], 'error');
    }
    $output = 
      '<event type="Live">
         <properties>
      ';

    $output .= $time;
    $output .= $house_id_info;
    $output .= '
           <switch>
             <source type="Fixed">';
    $output .= $fixed_port;
    $output .= 
      '</source>
                </switch>
            </properties>
        </event>';
  }
  else {
    $output = 
      '
       <event type="PrimaryVideo">
         <properties>
           <media mediaName="'.$data['house_id'].'"/>
           <mediaStream som="00:00:00;00">
             <segment type="Markup">
               <markup orderNo="1" name="Trimmed"/>
             </segment>
             <video jobType="Play"/>
             <allocation type="ListStream">
               <listStream type="Fixed" listStreamNo="0"/>
             </allocation>
           </mediaStream>
      ';
    
    $output .= $time;
    $output .= $house_id_info;


    $output .= 
      '
          </properties>
        </event>';
  }
  return $output;
  //return $data['title']." ".$data['project_id'].".".$data['episode']." ".
  //  date('Y-m-d',$data['start'])."T".date('H:i:s',$data['start']).
  //  " ".$data['runtime']." ".$data['format']."<br/>";
}

function mnn_programming_exports_livefeed_array($channel) {

  $sources = array(
	       "ST1" => '<fixed port="PGMSD-PCR1" device="EvertzEMR"/>',
	       "ST2" => '<fixed port="PGMSD-PCR2" device="EvertzEMR"/>',
	       "ST3" => '<fixed port="PGMSD-PCR3" device="EvertzEMR"/>',
	       "ST4" => '<fixed port="PGMSD-PCR4" device="EvertzEMR"/>',
	       "FH1" => '<fixed port="FH1 SD" device="EvertzEMR"/>',
	       "FH2" => '<fixed port="FH2 SD" device="EvertzEMR"/>',
	       "FH3" => '<fixed port="FH3 SD" device="EvertzEMR"/>',
	       "FH4" => '<fixed port="FH4 SD" device="EvertzEMR"/>',
	       "DNOW" => '<fixed port="RX-DNOW" device="EvertzEMR"/>',
	       "ARCH" => '<fixed port="RX-ARCH" device="EvertzEMR"/>',
	       "FSTV" => '<fixed port="RX-DISH" device="EvertzEMR"/>',
	       "BKLN" => '<fixed port="RX-BCAT" device="EvertzEMR"/>',
	       "BRNX" => '<fixed port="RX-BXNET" device="EvertzEMR"/>',
	       );
    

  if($channel == 5) {
    $hd_sources = array(
           "ST1_HD" => '<fixed port="PGM-PCR1" device="EvertzEMR"/>',
           "ST2_HD" => '<fixed port="PGM-PCR2" device="EvertzEMR"/>',
           "ST3_HD" => '<fixed port="PGM-PCR3" device="EvertzEMR"/>',
           "ST4_HD" => '<fixed port="PGM-PCR4" device="EvertzEMR"/>',
           );
    $sources = array_merge($sources, $hd_sources); // merge the two
  }
   

  return $sources;    
}

function mnn_programming_exports_pebble_beach_download() {
  
  $content = $_SESSION['mnn_pebble_beach_content'];
  $date = $_SESSION['mnn_pebble_beach_date'];
  if (!$content || !$date) {
    return "Sorry, something bad happend, error code 366742";
  }

  $output = 
    '<?xml version="1.0" encoding="UTF-8"?>
       <marinaPlaylist version="3.1">
       <eventList>
    ';

  $output .= $content[arg(4)];

  $output .= 
    "  </eventList>
     </marinaPlaylist>";
  $filename = "pebble_beach-".date('Y-m-d',$date)."-channel".arg(4).".mpl";

  header('Content-type: text/plain');
  header('Content-Disposition: attachment; filename="'.$filename.'"');

  print $output;
}


