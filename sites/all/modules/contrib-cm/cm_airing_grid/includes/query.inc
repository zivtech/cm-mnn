<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */


/** Note, this function will only return one term */

function cm_airing_grid_get_tid_for_vid_and_nid($vid, $nid) {
    $sql = "SELECT tn.tid from {term_node} tn " .
        "INNER JOIN {term_data} td on td.tid = tn.tid " .
        "where tn.nid= %d and td.vid=%d LIMIT 1";
    return db_result(db_query($sql, $nid, $vid));
}


function cm_airing_grid_get_all_non_live_episode_nids_for_project($proj_nid) {
  // Only finds eps with the format taxon set
  $fvid = cm_airing_grid_get_format_vid();
  $sql = "SELECT ep_node.nid, nce.field_episode_number_value
          FROM {node_content_episode} nce 
          INNER JOIN {node} ep_node 
            ON nce.vid = ep_node.vid 
          INNER JOIN {relativity} proj_rel 
            ON proj_rel.nid = ep_node.nid 
          INNER JOIN {node} proj_node 
            ON proj_rel.parent_nid = proj_node.nid 
          LEFT JOIN {term_node} tn
            ON tn.nid = ep_node.nid
          LEFT JOIN {term_data} td
            ON tn.tid = td.tid
          LEFT JOIN {vocabulary} v
            ON td.vid = v.vid
          WHERE proj_node.nid = %d
            AND nce.field_check_out_date_value IS NULL
            AND nce.field_episode_number_value != 0
            AND v.vid = %d
            AND td.name != 'LIVE'
            ORDER BY nce.field_episode_number_value desc
           ";
  $db_result = db_query($sql, $proj_nid, $fvid);
  return $db_result;
}


function cm_airing_grid_get_grid_airings
    ($channel, $start_time, $end_time, $mysql_day_of_week, $sort = 'asc') {
    $device_vid = cm_airing_grid_get_vid_for_name('Device');
    $format_vid = cm_airing_grid_get_vid_for_name('Format');

    $query = "
    SELECT DISTINCT
    airing.airing_id as airing_id  
    , fdfad.field_airing_date_value as event_start
    , fdfad.field_airing_date_value2 as event_end
    , proj_node.nid as project_nid
    , proj_node.title
    , show_node.nid as episode_nid
    FROM
    {cm_airing} airing
    INNER JOIN {field_data_field_airing_date} fdfad
    ON (airing.airing_id = fdfad.entity_id AND fdfad.entity_type = 'airing')
    INNER JOIN {field_data_field_airing_show_ref} fdfasr
    ON (airing.airing_id = fdfasr.entity_id AND fdfasr.entity_type = 'airing')
    INNER JOIN {node} show_node
    ON (fdfasr.field_airing_show_ref_target_id = show_node.nid 
      AND show_node.type = 'cm_show')
    INNER JOIN {field_data_field_airing_project_ref} fdfapr
    ON (airing.airing_id = fdfapr.entity_id AND fdfapr.entity_type = 'airing')
    INNER JOIN {node} proj_node
    ON (fdfapr.field_airing_project_ref_target_id = proj_node.nid and proj_node.type = 'cm_project')  
    INNER JOIN {field_data_field_airing_channel} fdfac
    ON (fdfac.entity_id = airing.airing_id and fdfac.entity_type = 'airing')    
    WHERE fdfad.field_airing_date_value2 > '" . date('Y-m-d H:i:s', $start_time) . "'
    AND fdfad.field_airing_date_value < '" . date('Y-m-d H:i:s', $end_time) . "'
    AND fdfac.field_airing_channel_tid = $channel
    AND (DAYOFWEEK(fdfad.field_airing_date_value) =
    $mysql_day_of_week
    OR DAYOFWEEK(fdfad.field_airing_date_value2) =
    $mysql_day_of_week)
    AND EXTRACT(HOUR FROM fdfad.field_airing_date_value) <=" .
    date('H', $end_time)."
    AND ((EXTRACT(HOUR FROM fdfad.field_airing_date_value2) >=".
      date('H', $start_time).") OR 
    ( DAYOFWEEK(fdfad.field_airing_date_value2) !=  
      DAYOFWEEK(fdfad.field_airing_date_value) ))

    		UNION
    		
    SELECT DISTINCT
    airing.airing_id as airing_id  
    , fdfad.field_airing_date_value as event_start
    , fdfad.field_airing_date_value2 as event_end
    , proj_node.nid as project_nid
    , proj_node.title
    , NULL
    FROM
    {cm_airing} airing
    INNER JOIN {field_data_field_airing_date} fdfad
    ON (airing.airing_id = fdfad.entity_id AND fdfad.entity_type = 'airing')
    INNER JOIN {field_data_field_airing_project_ref} fdfapr
    ON (airing.airing_id = fdfapr.entity_id AND fdfapr.entity_type = 'airing')
    INNER JOIN {node} proj_node
    ON (fdfapr.field_airing_project_ref_target_id = proj_node.nid and proj_node.type = 'cm_project')   
    INNER JOIN {field_data_field_airing_channel} fdfac
    ON (fdfac.entity_id = airing.airing_id and fdfac.entity_type = 'airing')    
    WHERE fdfad.field_airing_date_value2 > '" .date('Y-m-d H:i:s', $start_time) . "'
    AND fdfad.field_airing_date_value < '" . date('Y-m-d H:i:s', $end_time) . "'
    AND fdfac.field_airing_channel_tid = $channel
    AND (DAYOFWEEK(fdfad.field_airing_date_value) =
    $mysql_day_of_week
    OR DAYOFWEEK(fdfad.field_airing_date_value2) =
    $mysql_day_of_week)
    AND EXTRACT(HOUR FROM fdfad.field_airing_date_value) <=" .
    date('H', $end_time)."
    AND ((EXTRACT(HOUR FROM fdfad.field_airing_date_value2) >=".
      date('H', $start_time).") OR 
    ( DAYOFWEEK(fdfad.field_airing_date_value2) !=  
      DAYOFWEEK(fdfad.field_airing_date_value) ))";
     
  $query = "SELECT * from ($query) as air_join";
  if ($sort) {
    $query .= " order by air_join.event_start $sort";
  }
//file_put_contents('/tmp/mvc-query-log', $query."\n");
  $db_result = db_query($query); 

  return $db_result;
  
}

function cm_airing_grid_get_episode_length($episode_nid) {
	/*
	 * Original query, preserved for Mark & Eric
	*
	
	$sql = "SELECT (nce.field_timecode_out_value - nce.field_timecode_in_value)
         FROM {node_content_episode} nce 
         INNER JOIN {node} n ON nce.vid=n.vid
         INNER JOIN {relativity} proj_rel
           ON proj_rel.nid=nce.nid
         WHERE nce.nid = $episode_nid";
 */
	$sql = "SELECT (fdfad.field_airing_date_value2 - fdfad.field_airing_date_value)
	FROM {field_data_field_airing_date} fdfad
	WHERE fdfad.entity_id = $episode_nid";
	
  return db_query($sql);
}


function cm_airing_grid_check_for_overlap_by_node(&$node, $tid = NULL) {
  // FIXME: this cast shouldn't be needed, but we're getting strings instead of
  // ints somehow...
  $es = (int) $node->event_start;
  $ee = (int) $node->event_end;
  $nid = $node->nid ? $node->nid : 0;
  /*
   * Original query, preserved for Mark & Eric
  *
  
  $query = "SELECT e.*, n.title, n.nid FROM {node} n 
            INNER JOIN {event} e ON e.nid = n.nid 
            INNER JOIN {term_node} tn ON n.nid=tn.nid 
            INNER JOIN {term_data} td ON tn.tid=td.tid 
            INNER JOIN {vocabulary} v ON td.vid=v.vid 
            WHERE tn.tid = %d
            AND n.type = '%s'
            AND ((e.event_start <= %d AND
                  e.event_end   >  %d )
              OR (e.event_start <  %d AND
                  e.event_end   >= %d )
              OR (e.event_start >= %d AND
                  e.event_end   <= %d ))
            AND e.nid != %d 
            ORDER BY e.event_start asc";
  */

  $query = "SELECT e.*, n.title, n.nid FROM {node} n
  INNER JOIN {term_node} tn ON n.nid=tn.nid
  INNER JOIN {term_data} td ON tn.tid=td.tid
  INNER JOIN {vocabulary} v ON td.vid=v.vid
  WHERE tn.tid = %d
  AND n.type = '%s'
  AND ((fdfad.field_airing_date_value <= %d AND
  fdfad.field_airing_date_value2   >  %d )
  OR (fdfad.field_airing_date_value <  %d AND
  fdfad.field_airing_date_value2   >= %d )
  OR (fdfad.field_airing_date_value >= %d AND
  fdfad.field_airing_date_value2   <= %d ))
  AND e.nid != %d
  ORDER BY fdfad.field_airing_date_value asc";
  
  
  $db_result = db_query($query, $tid, $node->type, $es, $es, $ee, $ee, $es, $ee, $nid);
  $obj_list = NULL;
  foreach ($db_result as $result){
    if(is_object($result)){
      $obj_list[] = $result;
    }
  }
  return $obj_list;
}

function cm_airing_grid_get_vid_for_name($name) {
	$global_var_name = 'vid_cache_'.$name;
	global $$global_var_name;
	if ($$global_var_name) {
		return $$global_var_name;
	}

	
	if ( $name ) {
		$query = "SELECT vid FROM {taxonomy_vocabulary} WHERE name=:s LIMIT 1";

		$db_result = db_query($query, array(':s' => $name))->fetchField();

		$$global_var_name = $db_result;
		return $db_result;
	}
	else {
		return false;
	}
}
