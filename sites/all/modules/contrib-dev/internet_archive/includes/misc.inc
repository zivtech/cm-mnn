<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_validate_url($url) {
  $headers = get_headers($url, 1);
  if ($headers[0] != 'HTTP/1.1 200 OK' && $headers[1] != 'HTTP/1.1 200 OK') {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Formats dates for display on the report pages.
 *
 * @param $rows
 *   An array of report rows with data data
 * @param $limit
 *   The number of rows that will be displayed on a single page
 *
 * @return
 *   An array of report rows with adjusted friendly date data
 */
function internet_archive_format_dates($rows) {
  $counter = 0;
  $formatted_rows = array();

  foreach ($rows as $key => $row) {
    $formatted_rows[$key] = $row;
    if (!$formatted_rows[$key][4]) {
      $formatted_rows[$key][4] = 'N/A';
    }
    else {
      $formatted_rows[$key][4] = date("m/d/y g:ia", $row[4]);
    }
    if (!$formatted_rows[$key][5]) {
      $formatted_rows[$key][5] = 'N/A';
    }
    else {
      $formatted_rows[$key][5] = date("m/d/y g:ia", $row[5]);
    }
    $counter++;
  }

  return $formatted_rows;
}

/**
 * Cleans characters that break S3 from the header values.
 *
 * @param $string
 *   A string.
 *
 * @return $string
 *   A string without most html tags, whitespace, line breaks, tabs.
 */
function internet_archive_clean_text($string) {
  $string = strip_tags($string, '<a><br>');
  $string = trim($string);
  $string = str_replace(array("\r\n", "\r", "\n", "\t"), ' ', $string);

  return $string;
}

/**
 * Creates a safe bucket/item name from plain text.
 * @param $text
 *   A string containing text to make into an item name.
 *
 * @return
 *   Same text stripped of all non-alphanumeric characters
 */
function internet_archive_create_item_name($text) {
  $clean = preg_replace("/[^a-z0-9_-]+/i", "_", $text);
  $clean = substr($clean, 0, 60);
  return strtolower($clean);
}

/**
 * Grabs a list of fields for use in a select list.
 *
 * @param $field_types
 *   An array of field types to select from.
 *
 * @return $possible_fields
 *   All fields that fit the specified field types.
 */
function internet_archive_field_select_options($field_types =
					       array('filefield',
						     'text',
						     'emvideo',
						     'file',
						     'media')) {
  $fields = field_info_fields();
 
  $possible_fields = array();
  foreach ($fields as $field_name => $field) {
    if (in_array($field['type'], $field_types)) {
      $possible_fields[$field_name] = $field_name;
    }
  }
  asort($possible_fields);
  return $possible_fields;
}

/**
 * Workaround for PHPs lame 2GB/4GB file_size issues
 * Created with help from Ray Tiley, http://groups.drupal.org/user/81206
 */
function internet_archive_file_size($file) {
  $size = filesize($file);

  //Try to accomodate 32bit environments that allow shell command execution
  //and have ls
  //Theoretically PHP will always return a negative or 0 value if it cannot
  //handle the large file_size 
  if ($size < 0 || $size == 0) {
    $cmd = "ls -l " . escapeshellarg($file) . " | awk '{print $5}'";
    $ls_size = $cmd;
    if (is_numeric(trim($ls_size))) {
      $size = $ls_size;
    }
  }

  return $size;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_seconds_hhmmss($seconds) {
  $days = floor($seconds / 86400);
  $days_mod = $seconds % 86400;
  $hours = floor($days_mod / 3600);
  $hours_mod = $days_mod % 3600;
  $minutes = floor($hours_mod / 60);
  $minutes_mod = $hours_mod % 60;
  $seconds = floor($minutes_mod);

  if ($hours < 10) {
    $hours = '0' . $hours;
  }
  if ($minutes < 10) {
    $minutes = '0' . $minutes;
  }
  if ($seconds < 10) {
    $seconds = '0' . $seconds;
  }

  $formatted = $hours . ':' . $minutes . ':' . $seconds;
  return $formatted;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function internet_archive_help_links($name, $type = 'icon') {
  switch ($type) {
    case 'icon':
      // TODO Please change this theme call to use an associative array
      //for the $variables parameter.
      $output = theme('advanced_help_topic', 'internet_archive', $name);
      break;
    case 'link':
      // TODO Please change this theme call to use an associative array for
      //the $variables parameter.
      $link = theme('advanced_help_topic', 'internet_archive', $name, 'title');
      if (!$link) {
        $link = t('Please install the ') .
	  l('advanced help module', 'http://drupal.org/project/advanced_help')
	  . t(' for better documentation on using this module.');
      }
      else {
        $link = t('Help on the: !link.', array('!link' => $link));
      }
      $output = $link;
      break;
    case 'text':
      $output = t('Click the ? icon above for more help on this page.');
      break;
    case 'full':
      // TODO Please change this theme call to use an associative array for
      //the $variables parameter.
      $output = theme('advanced_help_topic', 'internet_archive', $name);
      $output .= '&nbsp;' .
	t('Click the help icon to view more details on this page.');
      break;
  }

  return $output;
}

/**
 * @param $file_name 
 *  -the file we are going to generate a md5 for
 *
 * @return $md5
 *  -a md generated hash of the file name
 */
function internet_archive_get_md5_for_file($file_name, $file_location) {
  if ($file_location == ARCHIVE_FILE_LOCATION_REMOTE) {
    $prefix = variable_get('internet_archive_remote_prefix', FALSE);
    $append = variable_get('internet_archive_remote_append', FALSE);
    $remote_file_name = $prefix . $file_name . $append;
    $remote_url = variable_get('internet_archive_server_url', FALSE);
    $url_parts = parse_url($remote_url);
    $file_name = "http://" . $url_parts['host'] . '/' . $remote_file_name;
    return md5_file($file_name);
  }

  if (md5_file($file_name)) {
    return md5_file($file_name);
  }
 
  return md5($file_name);
}
