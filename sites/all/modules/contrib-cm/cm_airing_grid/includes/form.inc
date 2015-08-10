<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */
 
 
function cm_airing_grid_grid_header_form ($form, &$form_state, $channel, 
					  $channels, $channel_tid, $start_time,
					  $end_time, $mysql_day_of_week, 
					  $collapsed = false, 
					  $grid_mode = "filter_end") {
	
  global $base_url;

  $form['#action'] = url($base_url.'/admin/manage/scheduling');
  	
  $form['collapsed_grid_header'] = array(
    '#title' => "<b>".t("Navigation")."</b>",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  ); 
  
	  
  $form['collapsed_grid_header']['cm_airing_grid_search_box_title'] = array(
    '#type' => 'markup',
    '#markup' => t("Grid Parameters"),
    '#prefix' => '<div class="cm-airing-grid-nav-box"><table>
                  <tr><td colspan=5><h2>',
    '#suffix' => '</tr></h2></tr>',
  );
  
 
  cm_airing_grid_get_date_options($years, $months, $days,
    $hours, $minutes, $ampm);

 
  $form['collapsed_grid_header']['cm_airing_grid_grid_hour'] = array(
    '#type' => 'select',
    '#title' => t('Hour'),
    '#default_value' => date('h', $start_time),
    '#options' => $months,
    '#prefix' => '<td>',
    '#suffix' => '</td>',
  );
  $form['collapsed_grid_header']['cm_airing_grid_grid_ampm'] = array(
    '#type' => 'select',
    '#title' => t('am/pm'),
    '#default_value' => date('a', $start_time),
    '#options' => $ampm,    
    '#prefix' => '<td>',
    '#suffix' => '</td>',
  );
  $form['collapsed_grid_header']['cm_airing_grid_grid_month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#default_value' => date('m', $start_time),
    '#options' => $months,    
    '#prefix' => '<td>',
    '#suffix' => "</td>",
  );
  $form['collapsed_grid_header']['cm_airing_grid_grid_day'] = array(
    '#type' => 'select',
    '#title' => t('Day'),
    '#default_value' => date('d', $start_time),
    '#options' => $days,
    '#prefix' => '<td>',
    '#suffix' => "</td>",
  );
  $form['collapsed_grid_header']['cm_airing_grid_grid_year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#default_value' => date('Y', $start_time),
    '#options' => $years,
    '#prefix' => '<td>',
    '#suffix' => "</td>",
  );
  $form['collapsed_grid_header']['cm_airing_grid_channel_select'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#default_value' => $channel_tid,
    '#options' => $channels,
    '#prefix' => '</tr><tr><td colspan=2>',
    '#suffix' => "</td>",
  );
  $form['collapsed_grid_header']['cm_airing_grid_grid_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#default_value' => $grid_mode,
    '#options' => array("filter_end"=>"Series Mode", 
      "show_all"=>"Fill Mode/Show All"),
    '#prefix' => '<td colspan=2>',
    '#suffix' => "</td>",
  );
  $form['collapsed_grid_header']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#prefix' => '<td>',
    '#suffix' => '</td></tr></table></div>',
    );

  
  $markup = cm_airing_grid_get_tabs($channel, $start_time, $end_time, 
				    $mysql_day_of_week, $grid_mode);
  
  $form['collapsed_grid_header']['timeblock_tabs'] = array(
    '#type' => 'markup',
    '#markup' => $markup,
  );    
  
  return $form;   
} 

function cm_airing_grid_get_grid_header($channel, $start_time, $end_time, 
					$mysql_day_of_week, $collapsed = false,
					$grid_mode = "filter_end") {
  $channels = 
    cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());

  $channel_number = preg_replace("/Channel /", "", $channel);
  $channel_tid = array_search($channel, $channels);
  
  $form_header = "<h1>$channel Grid ";
  $form_header .= ": ".date('l F j, Y', $start_time)."</h1>";
		
  $form_header .=
    theme('table', array('rows' => $rows, 'attributes' => 
			 array('class' => 'cm_airing_grid_grid_header')));
  
  $form_header .= "<div align='center'>".
    drupal_render(drupal_get_form('cm_airing_grid_grid_header_form', $channel, 
				  $channels, $channel_tid, $start_time, 
				  $end_time, $mysql_day_of_week, $collapsed,
				  $grid_mode))."</div>";
  
  return $form_header;
		 
}


function cm_airing_grid_grid_header_form_submit($form, &$form_state) {
  $channels = 
    cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());
  $hour = _class_scheduler_fix_hour
    ($form_state['values']['cm_airing_grid_grid_hour'],
     $form_state['values']['cm_airing_grid_grid_ampm']);  

  $new_time = mktime($hour, 0, 0, 
		     $form_state['values']['cm_airing_grid_grid_month'],
		     $form_state['values']['cm_airing_grid_grid_day'], 
		     $form_state['values']['cm_airing_grid_grid_year']);
  $channel_tid = $form_state['values']['cm_airing_grid_channel_select'];
  $channel = $channels[$channel_tid];
  $channel = preg_replace("/Channel /", "", $channel);
  $mode = $form_state['values']['cm_airing_grid_grid_mode'];
 
  drupal_goto ("admin/manage/scheduling/$channel/$new_time/$hour/$mode");
}
 


function cm_airing_grid_airing_detail_form($airing_nid)  {
  $output = "TODO FIXME IMPLEMENT ME";
  return $output;
}
