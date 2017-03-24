<?php


/**
 * submit hookd of cma_program_import_form
 */
function cma_program_import_process_csv($import_node) {
  $fid = $import_node->field_uploader_file[LANGUAGE_NONE][0]['fid'];

  $file_record = file_load($fid);

  $file_uri = $file_record->uri;
  $file_path = str_replace('public://', '', $file_uri);
  $file_dir_path = variable_get('file_directory_path', conf_path() .'/files');
  $base_path = $_SERVER['DOCUMENT_ROOT'];

  $file_path_full = $base_path."/".$file_dir_path."/".$file_path;

  include_once "/var/www/cm-stage.mnn.org/sites/all/libraries/csv2array.php";
  $csvArray = ImportCSV2Array($file_path_full);

  return($csvArray);

}


