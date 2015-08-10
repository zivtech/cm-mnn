<?php
/**
 * @file
 * mnn_class_import.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function mnn_class_import_field_default_fields() {
  $fields = array();

  // Exported field: 'node-civicrm_multiday_event-field_trainer'
  $fields['node-civicrm_multiday_event-field_trainer'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_trainer',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'module' => 'user_reference',
      'settings' => array(
        'profile2_private' => FALSE,
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          4 => 0,
          6 => 0,
          7 => 0,
          8 => 0,
          10 => 0,
          11 => 0,
          14 => 0,
          15 => 0,
          16 => 0,
          17 => 0,
          18 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
          30 => 0,
          31 => 0,
          32 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'civicrm_multiday_event',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => 11,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_trainer',
      'label' => 'Trainer',
      'required' => FALSE,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '13',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Trainer');

  return $fields;
}
