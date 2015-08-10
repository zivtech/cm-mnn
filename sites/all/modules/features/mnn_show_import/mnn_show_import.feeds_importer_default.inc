<?php
/**
 * @file
 * mnn_show_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_show_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'mnn_show_import';
  $feeds_importer->config = array(
    'name' => 'Show Import',
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
        'content_type' => 'cm_show',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'created',
            'target' => 'created',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'exec_producer_id',
            'target' => 'uid',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'language',
            'target' => 'taxonomy_pbcore_languages',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'device',
            'target' => 'field_device',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'runtime',
            'target' => 'field_runtime',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'content_type',
            'target' => 'field_pbcore_genres',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'format',
            'target' => 'field_format',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'topic',
            'target' => 'field_topic',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'schedule_type',
            'target' => 'taxonomy_cm_project_types',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'live_device',
            'target' => 'field_device',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'reject_reason',
            'target' => 'field_reject_reason',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'delivered_via',
            'target' => 'field_delivered_via',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'episode_id',
            'target' => 'guid',
            'unique' => 1,
          ),
          14 => array(
            'source' => 'time_code_in',
            'target' => 'field_timecode_in',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'time_code_out',
            'target' => 'field_timecode_out',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'transcoded_time_code_in',
            'target' => 'field_trans_timecode_in',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'transcoded_time_code_out',
            'target' => 'field_trans_timecode_out',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'runtime_type',
            'target' => 'field_runtime',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'episode_number',
            'target' => 'field_episode_number',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['mnn_show_import'] = $feeds_importer;

  return $export;
}
