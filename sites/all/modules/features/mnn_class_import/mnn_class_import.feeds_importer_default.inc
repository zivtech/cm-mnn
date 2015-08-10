<?php
/**
 * @file
 * mnn_class_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_class_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'classes_import';
  $feeds_importer->config = array(
    'name' => 'Classes Import',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'civicrm_multiday_event',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(
          0 => array(
            'source' => 'class_id',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'created',
            'target' => 'created',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'certification',
            'target' => 'field_civicrm_group',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'number_attended',
            'target' => 'field_class_attendance',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'building',
            'target' => 'field_building',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'department',
            'target' => 'field_department',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'class_start_day',
            'target' => 'field_civicrm_multiday_session:field_session_date:start',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['classes_import'] = $feeds_importer;

  return $export;
}
