<?php
/**
 * @file
 * mnn_reservable_node_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mnn_reservable_node_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'non_reservable_equipment_import';
  $feeds_importer->config = array(
    'name' => 'Non-Reservable Equipment Import',
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
        'content_type' => 'equipment_non_reservable_',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['non_reservable_equipment_import'] = $feeds_importer;

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'non_reservable_facility_import';
  $feeds_importer->config = array(
    'name' => 'Non-Reservable Facility Import',
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
        'content_type' => 'facility_non_reservable_',
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['non_reservable_facility_import'] = $feeds_importer;

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'reservable_equipment_import';
  $feeds_importer->config = array(
    'name' => 'Reservable Equipment Import',
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
      'plugin_key' => 'ReservationsNodeProcessor',
      'config' => array(
        'content_type' => '59th_facility_cds_fac_',
        'expire' => -1,
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'type',
            'target' => 'type',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          3 => array(
            'source' => 'accessories',
            'target' => 'field_accessories',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'price',
            'target' => 'field_cost',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'building',
            'target' => 'field_building',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'department_name',
            'target' => 'field_department',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'in_service_date',
            'target' => 'field_in_service_ate:start',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'inventory_name',
            'target' => 'field_inventory',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'inventory_number',
            'target' => 'field_inventory_number',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'manufacturer',
            'target' => 'field_manufacturer',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'model',
            'target' => 'field_model',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'serial_number',
            'target' => 'field_serial_number',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'warranty_expiration_date',
            'target' => 'field_warrenty_expiration_date:end',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'default_availability',
            'target' => 'reservations_default_availability',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'promoted',
            'target' => 'promote',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => 0,
        'input_format' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['reservable_equipment_import'] = $feeds_importer;

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'reservable_facility_import';
  $feeds_importer->config = array(
    'name' => 'Reservable Facility Import',
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
      'plugin_key' => 'ReservationsNodeProcessor',
      'config' => array(
        'content_type' => '59th_facility_cds_fac_',
        'expire' => -1,
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'type',
            'target' => 'type',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          3 => array(
            'source' => 'accessories',
            'target' => 'field_accessories',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'price',
            'target' => 'field_cost',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'building',
            'target' => 'field_building',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'department_name',
            'target' => 'field_department',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'in_service_date',
            'target' => 'field_in_service_ate:start',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'inventory_name',
            'target' => 'field_inventory',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'inventory_number',
            'target' => 'field_inventory_number',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'manufacturer',
            'target' => 'field_manufacturer',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'model',
            'target' => 'field_model',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'serial_number',
            'target' => 'field_serial_number',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'warranty_expiration_date',
            'target' => 'field_warrenty_expiration_date:end',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'default_availability',
            'target' => 'reservations_default_availability',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'promoted',
            'target' => 'promote',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => 0,
        'input_format' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['reservable_facility_import'] = $feeds_importer;

  return $export;
}
