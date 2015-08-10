<?php
/**
 * @file
 * mnn_project_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_project_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'projects';
  $feeds_importer->config = array(
    'name' => 'Projects',
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
        'delimiter' => ';',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'cm_project',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(
          0 => array(
            'source' => 'project_id',
            'target' => 'nid',
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
            'source' => 'exec_producer_id',
            'target' => 'uid',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'narrative',
            'target' => 'field_description',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'public_contact_info',
            'target' => 'field_public_contact',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'interval',
            'target' => 'field_interval',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'department',
            'target' => 'field_department',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'device',
            'target' => 'field_device',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'runtime',
            'target' => 'field_runtime',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'interval',
            'target' => 'field_interval',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'language',
            'target' => 'field_pbcore_languages',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
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
  $export['projects'] = $feeds_importer;

  return $export;
}
