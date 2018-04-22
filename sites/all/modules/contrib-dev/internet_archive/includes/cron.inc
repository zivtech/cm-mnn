<?php

/**
 * Implements hook_cron().
 */
function internet_archive_cron() {
  global $user;
  $original_user = $user;

  $user = user_load(1);
  //validate previous transfers
  internet_archive_validate_transfers();

  if (variable_get('internet_archive_queue_harvest_cron', FALSE)) {
    //Add new files to queue if available
    internet_archive_harvest_files();
  }
  
  //Check for new derivations to update.
  internet_archive_store_derivatives();

  $user = $original_user;
}


/** * Implementation of hook_cron_queue_info() */
function internet_archive_cron_queue_info() {
  // This is the callback function for each queue item.
  $queues['internet_archive_send_file'] =
    array(
	  'worker callback' => 'internet_archive_send_file_run',
	  'time' => 180, 
	  );
  return $queues;
}

/**
 * Function is called by cron to act on local and remote files and push
 * the data up to archive.org
 */
function internet_archive_send_file_run($item) {
  global $internet_archive_transfer_count;

  $internet_archive_transfer_count = $internet_archive_transfer_count ?
    $internet_archive_transfer_count : 0;

  $internet_archive_transfer_count ++;

  $limit = variable_get('internet_archive_transfer_limit', '1');

  if ($internet_archive_transfer_count > $limit) {
    throw new Exception('Item will remain in the queue.');
  }

  //FETCH DAYS OF WEEK AND CHECK
  if (!internet_archive_day_check()) {
    throw new Exception('Item will remain in the queue.');
  }
  
  //FETCH HOUR OF DAY AND CHECK
  if (!internet_archive_hour_check()) {
    throw new Exception('Item will remain in the queue.');
  }

  //SENDING THE ITEM WITH FILE NAME, FIELD NAME, AND FILE LOCATION
  internet_archive_default_send($item);
  
}

/**
 * Compares current hour to the hour range set on the internet archive
 * queue settings page.
 *
 * @return
 *   TRUE if the current hour is within range, FALSE if not
 */
function internet_archive_hour_check() {
  $start_hour = variable_get('internet_archive_queue_start_time', 'none');
  if ($start_hour == 'none') {
    return TRUE;
  }

  $end_hour = variable_get('internet_archive_queue_end_time', 'none');
  $current_hour = date('G');
  $available_hours = array();

  //quick loop to find valid hours
  $set_hour = $start_hour;
  while ($set_hour != $end_hour) {
    $available_hours[] = $set_hour;
    switch ($set_hour) {
      case 23:
        $set_hour = 0;
        break;
      default:
        $set_hour++;
        break;
    }
  }

  if (in_array($current_hour, $available_hours)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Compares current day to the day/s set on the internet archive
 * queue settings page.
 *
 * @return
 *   TRUE if the current day matches one chosen, FALSE if not
 */
function internet_archive_day_check() {
  $allowed_days = variable_get('internet_archive_queue_days', 'none');
  $current_day = strtolower(date('l'));

  if (empty($allowed_days) || in_array($current_day, $allowed_days)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Queue worker function to send files
 *
 * @param $queue_item array of file_name, field_name, and file_location
 * @return $status
 *   True or False
 */
function internet_archive_transfer_file_worker($queue_item) {
  $status = internet_archive_default_send($queue_item);
  return $status;
}

