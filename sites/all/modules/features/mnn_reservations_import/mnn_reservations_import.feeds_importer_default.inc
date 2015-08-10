<?php
/**
 * @file
 * mnn_reservations_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_reservations_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'mnn_reservations_import';
  $feeds_importer->config = array(
    'name' => 'mnn_reservations_import',
    'description' => 'Import Reservation nodes with association to Producer, Project, and Inventory',
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
        'content_type' => 'reservations_reservation',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(
          0 => array(
            'source' => 'reservation_id',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'reservation_id',
            'target' => 'field_reservation_id',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'producer_id',
            'target' => 'uid',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'project_id',
            'target' => 'field_project_id',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'reserve_start_time',
            'target' => 'field_reservations_date:start',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'reserve_end_time',
            'target' => 'field_reservations_date:end',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'inventory_name',
            'target' => 'field_reservations_inventory',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
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
  $export['mnn_reservations_import'] = $feeds_importer;

  return $export;
}
