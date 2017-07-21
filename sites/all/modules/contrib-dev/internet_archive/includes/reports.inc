<?php

/**
 * Loads the general report page.
 *
 */
function internet_archive_reports_home() {
  return internet_archive_reports_log();
}

/**
 * Produces the statistical report at admin/reports/internet_archive/stats with
 * some aggregate upload and download information
 */
function internet_archive_reports_statistics() {
  $header = array('Stat', 'Value');
  $count = db_query("SELECT COUNT(*) FROM {internet_archive}")->fetchField();

  if (module_exists('internet_archive_download')) {
    $file_rows[] =
      array('Downloaded',
	    db_query("SELECT COUNT(*) FROM {internet_archive_download} " .
		     "WHERE status = :status",
		     array(':status' => 'downloaded'))->fetchField());
  }
  $file_rows[] =
    array('Derived',
	  db_query("SELECT COUNT(*) FROM {internet_archive} " .
		   "WHERE status = :status",
		   array(':status' => 'derived'))->fetchField());

  $file_rows[] =
    array('Validated',
	  db_query("SELECT COUNT(*) FROM {internet_archive} " .
		   "WHERE status = :status",
		   array(':status' => 'validated'))->fetchField());
  
  $file_rows[] =
    array('Transferred',
	  db_query("SELECT COUNT(*) FROM {internet_archive} " .
		   "WHERE status = :status",
		   array(':status' => 'transferred'))->fetchField());
  
  $file_rows[] =
    array('Transferring',
	  db_query("SELECT COUNT(*) FROM {internet_archive} ".
		   "WHERE status = :status",
		   array(':status' => 'transferring'))->fetchField());

  $file_rows[] =
    array('Queued',
	  db_query("SELECT COUNT(*) FROM {internet_archive} ".
		   "WHERE status = :status",
		   array(':status' => 'queued'))->fetchField());
  
  $file_rows[] =
    array('Failed',
	  db_query("SELECT COUNT(*) FROM {internet_archive} " .
		   "WHERE status = :status",
		   array(':status' => 'failed'))->fetchField());
  
  $file_table = theme('table',array('header'=> $header, 'rows' => $file_rows));

  $total = db_query("SELECT sum(file_size) FROM {internet_archive} ".
		    "WHERE status = :status OR status = :status",
		    array(':status' => 'validated',
			  ':status' => 'derived'))->fetchField();
  $transfer_rows[] = array('Total Data Uploaded',
			   round($total / 1024 / 1024) . ' MB');
  
  $transfer_rows[] = array('Upload Rate Average (overall)',
			   internet_archive_transfer_rate($limit = 0) .
			   ' Mb/sec | ' .
			   internet_archive_transfer_rate($limit = 0, 'time').
			   ' min/file');
  if ($count > 20) {
    $transfer_rows[] = array('Upload Rate Average (last 15 transfers)',
			     internet_archive_transfer_rate(15) .
			     ' Mb/sec | ' .
			     internet_archive_transfer_rate($limit = 15,
							    'time') .
			     ' min/file');
  }
  if ($count > 55) {
    $transfer_rows[] = array('Upload Rate Average (last 50 transfers)',
			     internet_archive_transfer_rate(50) . ' Mb/sec | '.
			     internet_archive_transfer_rate($limit = 50,
							    'time') .
			     ' min/file');
  }

  $val_plus_derived = db_query("SELECT avg(file_size) " .
			       "FROM {internet_archive} " .
			       "WHERE status = :status OR status = :status",
			       array(':status' => 'validated',
				     ':status' => 'derived'))->fetchField();
  
  $transfer_rows[] = array('Average File Size',
			   round($val_plus_derived / 1024 / 1024, 4) . ' MB');

  $transfer_table = theme('table', array('header' => $header,
					 'rows' => $transfer_rows));

  if (module_exists('internet_archive_download')) {
    $download_stats = internet_archive_download_statistics();
    $download_table = internet_archive_download_table($download_stats);
  }

  $output = '<h3>Internet Archive Statistics</h3><br />';
  $output .= '<div class="admin-panel">';
  $output .= '<h4 style="border-bottom:1px solid black">File Status</h4>';
  $output .= $file_table;
  $output .= '</div>';
  $output .= '<div class="admin-panel">';
  $output .= '<h4 style="border-bottom:1px solid black">Upload Stats</h4>';
  $output .= $transfer_table;
  $output .= '</div>';
  if ($download_table) {
    $output .= '<div class="admin-panel">';
    $output .= '<h4 style="border-bottom:1px solid black">Download Stats</h4>';
    $output .= $download_table;
    $output .= '</div>';
  }

  return $output;
}

/**
 * Produces the upload report at admin/reports/internet_archive showing a 
 * summary of recent file uploads
 */
function internet_archive_reports_log() {
  $output .= 'Typical file status process is: Queued -> Transferring -> '.
    'Transferred -> Validated- > Derived';
  
  $output .= '<br />For information on Archive.org status, please ' .
    '<a href="http://blog.archive.org/">checkout their blog</a> and/or ' .
    'subscribe to <a href="http://twitter.com/internetarchive">their '.
    'twitter feed</a>.<br /><br />';
  $output .= internet_archive_uploads();
  return $output;
}

/**
 * Produces the system messages report at admin/reports/internet_archive/log 
 * showing a listing of all system log messages sorted by date.
 */
function internet_archive_reports_system_messages() {
  if (arg(4) == 'all') {
    $output .=
      t('All status messages recorded by the system are shown below.') .
      l(' Click here to view only warnings and errors.',
	'admin/reports/internet_archive/log');
  }
  else {
    $output .=
      t('All warning and error level status messages recorded by the system '.
	'are shown below.') .
      l(' Click here to view all messages, including notices.',
	'admin/reports/internet_archive/log/all');
  }
  $output .= internet_archive_log_view('global');
  return $output;
}

/**
 * Builds the table view of recent uploads for the upload report.
 *
 * @param $limit
 *   Number of upload entries to show on a single page.
 */
function internet_archive_uploads() {
  $page_number = arg(3);
  $results_per_page = variable_get('internet_archive_results_per_page', 100);

  if ($page_number && $page_number > 1) {
    $min = (($results_per_page) * $page_number) - $results_per_page;
  }
  else {
    $page_number = 1;
    $min = 0;
  }
  
  $result =db_query_range("SELECT * FROM {internet_archive} ".
			  "ORDER BY transfer_initiated DESC",
			  $min, $results_per_page);
  
  $header = array('ID', 'Node', 'File Name', 'File Size (MB)', 'Start',
		  'Finish', 'Time(min)<br />Rate(Mb/sec)', 'Status', 'Log');
  
  $rows = array();

  $has_results = FALSE;
  $count = 0;
  while ($archive_data = $result->fetchAssoc()) {
    $count ++;
    $has_results = TRUE;
    unset($transfer_time);
    unset($file_size);
    unset($transfer_rate);
    unset($file_name);

    if ($archive_data['date'] > 0 && $archive_data['transfer_initiated'] > 0) {
      $transfer_time =
	($archive_data['date'] - $archive_data['transfer_initiated']) / 60;
      $transfer_time =round($transfer_time, 2);
    }
    else {
      $transfer_time = 'N/A';
    }

    if ($archive_data['file_size'] > 0) {
      $file_size = round(($archive_data['file_size'] / 1024 / 1024), 2);
    }
    else {
      $file_size = 'N/A';
    }

    if ($archive_data['file_size'] > 0 && $archive_data['date'] > 0 &&
	$archive_data['transfer_initiated'] > 0) {

      $transfer_rate = $archive_data['file_size'] * 8 / 1000 / 1000;

      $transfer_rate = $transfer_rate /
	($archive_data['date'] - $archive_data['transfer_initiated']);
      
      $transfer_rate = round($transfer_rate, 4);
    }
    else {
      $transfer_rate = 'N/A';
    }

    if (strlen($archive_data['file_name']) > 40) {
      $file_name = substr($archive_data['file_name'], 0, 40) . '...';
    }
    else {
      $file_name = $archive_data['file_name'];
    }

    $rows[] =
      array(l($archive_data['tid'], 'http://www.archive.org/details/' .
	      $archive_data['item']),
	    l($archive_data['nid'], 'node/' . $archive_data['nid']),
	    $file_name, $file_size, $archive_data['transfer_initiated'],
	    $archive_data['date'],
	    $transfer_time . '<br />' . $transfer_rate,
	    $archive_data['status'],
	    l('View', 'ia/log/' . $archive_data['tid']));
  }

  if ($has_results) {
    $rows = internet_archive_format_dates($rows, $limit);
    $table = theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $table = "<h3>" . t("There are no more results to show.") . "</h3>";
  }

  if ($page_number > 1) {
    $prev_page = $page_number - 1;
    $table .= "<br/>" .
      l("<<<<< Previous ", "admin/reports/internet_archive/" . $prev_page);
    $table .= "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
  }

  if ($has_results && $count == $results_per_page) {
    $next_page = $page_number + 1;
    $table .= l(" Next >>>>>>", "admin/reports/internet_archive/".$next_page);
  }
  return $table;
}

/**
 * Calculates the overall transfer rate for recent transfers
 *
 * @param $limit
 *   The number of transfers to use in the calculation, starts at most recent
 *   and goes backwards
 * @param $format
 *   The method of calculation, either rate or time.
 *
 * @return
 *   A rounded decimal containing either the rate in mbits/sec or total time 
 *   in minutes.
 */
function internet_archive_transfer_rate($limit = 0, $format = 'rate') {
  $total_seconds = 0;
  $total_bytes = 0;
  $counter = 0;

  $result =
    db_query("SELECT * FROM {internet_archive} ".
	     "WHERE status = :status OR status = :status ".
	     "ORDER BY tid DESC",
	     array(':status' => 'validated', ':status' => 'derived'));
  
  while ($archive_data = $result->fetchAssoc() ) {
    if ($archive_data['transfer_initiated'] != 0 &&
	$archive_data['date'] != 0 && $archive_data['file_size'] != 0) {

      $total_bytes = $total_bytes + $archive_data['file_size'];
      $total_seconds = $total_seconds + ($archive_data['date'] -
					 $archive_data['transfer_initiated']);
      $counter++;
    }

    if ($limit != 0 && $counter >= $limit) {
      break;
    }
  }

  switch ($format) {
    case 'rate':
      if ($total_bytes && $total_seconds > 0) {
        $bits = $total_bytes * 8;
        $mbits = $bits / 1000 / 1000;
        $mbits_sec = $mbits / $total_seconds;
        return round($mbits_sec, 4);
      }
      else {
        return 'N/A';
      }
      break;

    case 'time':
      if ($total_seconds > 0) {
        $minutes = $total_seconds / $counter / 60;
        return round($minutes, 2);
      }
      else {
        return 'N/A';
      }
      break;
  }
}

/**
 * Log function used to record all updates associated with file transfers
 */
function internet_archive_log($data) {
  //dsm($data, "Log entry!!!!!!!!!!!!!!!!!!!!!!!!!");
  if ($data['tid'] && $data['message'] && $data['type']) {
    $id = db_insert('internet_archive_log')
      ->fields(array(
		     'tid' => $data['tid'],
		     'message' => $data['message'],
		     'message_data' => serialize($data['message_data']),
		     'type' => $data['type'],
		     'date' => REQUEST_TIME,
		     ))
      ->execute();
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_log_table($type = 'tid', $identifier = NULL,
				    $show_only_errors_and_warnings = FALSE) {
  dsm('internet_archive_log_table');
  $args = array();
  switch ($type) {
    case 'tid':
      $where = ' WHERE log.tid=:tid';
      $args[':tid'] = $identifier;
      break;
    case 'nid':
      $where = ' WHERE archive.nid=:nid';
      $args[':nid'] = $identifier;
      break;
    case 'global':
      if ($show_only_errors_and_warnings) {
        $where = " WHERE log.type='" . ARCHIVE_LOG_WARNING .
	  "' OR log.type='" . ARCHIVE_LOG_ERROR . "'";
      }
      break;
  }

  $query =
    "SELECT log.lid, log.tid, log.message, log.message_data, log.type,".
    "log.date ".
    "FROM {internet_archive_log} AS log ".
    "INNER JOIN {internet_archive} AS archive ON archive.tid=log.tid" .
    $where . " ORDER BY log.date DESC";

  $page_number = arg(4);
  $results_per_page = variable_get('internet_archive_results_per_page', 100);

  if ($page_number && $page_number > 1) {
    $min = (($results_per_page) * $page_number) - $results_per_page;
  }
  else {
    $page_number = 1;
    $min = 0;
  }
  
  $result = db_query_range($query, $min, $results_per_page, $args);

  while ($log_entry = $result->fetchAssoc()) {
    switch ($log_entry['type']) {
      case 1:
        $friendly_type = 'error';
        break;
      case 2:
        $friendly_type = 'warning';
        break;
      case 3:
        $friendly_type = 'notice';
        break;
    }

    $message = $log_entry['message'];
    $message .= "&nbsp;on <em>" . date("F j, Y, g:i a", $log_entry['date']) .
      "</em>";

    if (function_exists('krumo') && php_sapi_name() != 'cli') {
      $message .= '<br />';
      $message .= krumo_ob(unserialize($log_entry['message_data']));
    }

    $rows[] = array('data' => array(array('data'=>$message,
					  'class'=>array('')),
				    array('data'=>$friendly_type,
					  'class'=>array('ia-type')),
				    ),
		    'class' => array('ia-' . $friendly_type),
		    );
    
  }

  if (count($rows) > 0) {
    $header = array('Message', 'Type');

    $table = theme('table', array('header' => $header, 'rows' => $rows));

    if (!function_exists('krumo') || php_sapi_name() == 'cli') {
      $table .=
	t('For more detailed debugging, please install the devel module and '.
	  'return to this page.');
    }

    if ($page_number > 1) {
      $prev_page = $page_number - 1;
      $table .= "<br/>" .l("<<<<< Previous ",
			   "admin/reports/internet_archive/log/" . $prev_page);
      
      $table .= "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
    }
    
    if (count($rows) == $results_per_page) {
      $next_page = $page_number + 1;
      $table .= l(" Next >>>>>>", "admin/reports/internet_archive/log/".
		  $next_page);
    }
    
    return $table;
  }
  else {
    $prev_page = $page_number - 1;
    $link .= "<br/>" .l("<<<<< Previous ",
			"admin/reports/internet_archive/log/" . $prev_page);

    return t('No log information is currently available for this file.').$link;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_log_view($type = 'tid', $identifier = NULL) {
  dsm('internet_archive_log_view');

  if (user_access('access internet_archive logs')) {
    drupal_add_css(drupal_get_path('module', 'internet_archive') .
		   '/css/log.css');
    if ($type == 'tid') {
      $item = internet_archive_load_data_tid($identifier);
      $node = node_load($item['nid']);
      $output = '<br /><h3>Log for <i>' . $node->title . '</i> source file: ' .
	$item['in_path'] . '</h3>';

      $output .=
	l('Visit the node associated with this transfer',
	  'node/' . $item['nid'] . '/ia');
      
      $output .= ' | ' . l('View the main log page',
			   'admin/reports/internet_archive/log');
    }
    elseif ($type == 'nid') {
      $node = node_load($identifier);
      $output =
	'<h3>Combined log for all transfers associated with the '.
	'<i>' . $node->title . '</i> node</h3>';
    }
    elseif ($type == 'global') {
      $output = '<h3>Global message log</h3>';
    }

    $show_notice_config =
      variable_get('internet_archive_show_only_errors_and_warnings', FALSE);

    if (arg(3) == 'log' && $show_notice_config) {
      $show_only_errors_and_warnings = TRUE;
    }
    else {
      $show_only_errors_and_warnings = FALSE;
    }
    
    $output .= internet_archive_log_table($type, $identifier,
					  $show_only_errors_and_warnings);
    
    return $output;
  }
  else {
    drupal_access_denied();
  }

}

