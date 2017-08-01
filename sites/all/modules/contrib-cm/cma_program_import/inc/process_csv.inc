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
      $line_array[$key] = array_shift($line);
    }
    $csv_array[] = $line_array;
  }
  return($csv_array);

}


