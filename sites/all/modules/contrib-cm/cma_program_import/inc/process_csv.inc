<?php


/**
 * submit hookd of cma_program_import_form
 */
function cma_program_import_process_csv($import_node) {
  $fid = $import_node->field_uploader_file[LANGUAGE_NONE][0]['fid'];

  $file_record = file_load($fid);
  $file_handle = fopen($file_record->uri, 'r');

  $keys = fgetcsv($file_handle);
  while ($line = fgetcsv($file_handle)) {
    $line_array = array();
    foreach ($keys as $key) {
      $text = array_shift($line);

      $text = str_replace('&quot;', '"', $text);
      $text = str_replace('&#39;', "'", $text);
      $text = str_replace('&lt;', '<', $text);
      $text = str_replace('&gt;', '>', $text);
      $text = str_replace('&amp;', '&', $text);

      $strlen = strlen( $text );
      $safe_text = '';
      for( $i = 0; $i <= $strlen; $i++ ) {
	$char = substr( $text, $i, 1 );
	if (!ctype_space($char) && !ctype_alnum($char) && !ctype_punct($char)){
	  $safe_text .= '|';
	}
	else {
	  $safe_text .= $char;
	}
      }
      $safe_text = str_replace('|||', "'", $safe_text);
      $safe_text = str_replace('|', "", $safe_text);
      $line_array[$key] = $safe_text;
    }
    $csv_array[] = $line_array;
  }

  return($csv_array);

}


