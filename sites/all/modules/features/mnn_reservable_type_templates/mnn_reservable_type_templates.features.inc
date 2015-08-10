<?php
/**
 * @file
 * mnn_reservable_type_templates.features.inc
 */

/**
 * Implements hook_node_info().
 */
function mnn_reservable_type_templates_node_info() {
  $items = array(
    'equipment_template' => array(
      'name' => t('Equipment Template'),
      'base' => 'node_content',
      'description' => t('This content type acts as a template for all further equipment Content Types. By using the Sync Field functionality this Content Type will provide the basic set of fields for new equipment Content Types.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'facility_template' => array(
      'name' => t('Facility Template'),
      'base' => 'node_content',
      'description' => t('This content type acts as a template for all further facility Content Types. By using the Sync Field functionality this Content Type will provide the basic set of fields for new facility Content Types.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
