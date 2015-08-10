<?php
/**
 * @file
 * mnn_airing_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_airing_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'airing_import';
  $feeds_importer->config = array(
    'name' => 'Airing Import',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => 1,
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
      'plugin_key' => 'FeedsEntityProcessorAiring',
      'config' => array(
        'values' => array(),
        'mappings' => array(
          0 => array(
            'source' => 'start_time',
            'target' => 'field_airing_date:start',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'end_time',
            'target' => 'field_airing_date:end',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'channel',
            'target' => 'field_airing_channel',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'airing_type',
            'target' => 'field_airing_type',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'project_id',
            'target' => 'field_airing_project_ref',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'title',
            'target' => 'field_temp_title',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'airing_id',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'update_existing' => 0,
        'input_format' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '0',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['airing_import'] = $feeds_importer;

  return $export;
}
