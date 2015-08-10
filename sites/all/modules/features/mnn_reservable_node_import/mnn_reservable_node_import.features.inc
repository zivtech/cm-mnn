<?php
/**
 * @file
 * mnn_reservable_node_import.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mnn_reservable_node_import_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function mnn_reservable_node_import_node_info() {
  $items = array(
    'equipment_non_reservable_' => array(
      'name' => t('Equipment (Non-Reservable)'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'facility_non_reservable_' => array(
      'name' => t('Facility (Non-Reservable)'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
