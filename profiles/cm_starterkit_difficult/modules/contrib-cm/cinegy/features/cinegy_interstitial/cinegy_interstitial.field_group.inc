<?php
/**
 * @file
 * cinegy_interstitial.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function cinegy_interstitial_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_images|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_images';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials_content';
  $field_group->data = array(
    'label' => 'Bilder',
    'weight' => '11',
    'children' => array(
      0 => 'field_okv_image',
      1 => 'field_logo',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Bilder',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_images|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interstitials_admindata|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_interstitials_admindata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials';
  $field_group->data = array(
    'label' => 'Endast personal',
    'weight' => '15',
    'children' => array(
      0 => 'field_duration',
      1 => 'field_video_background',
      2 => 'field_staff_verified',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Endast personal',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_interstitials_admindata|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interstitials_content|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_interstitials_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials';
  $field_group->data = array(
    'label' => 'Innehåll',
    'weight' => '14',
    'children' => array(
      0 => 'field_cinegy_type_template',
      1 => 'group_templates_text',
      2 => 'group_images',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_interstitials_content|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interstitials_general|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_interstitials_general';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials';
  $field_group->data = array(
    'label' => 'Grundläggande',
    'weight' => '12',
    'children' => array(
      0 => 'group_audience',
      1 => 'field_type',
      2 => 'title',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Grundläggande',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_interstitials_general|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interstitials_schedule|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_interstitials_schedule';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials';
  $field_group->data = array(
    'label' => 'Schemaläggning',
    'weight' => '13',
    'children' => array(
      0 => 'field_start_date',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Schemaläggning',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_interstitials_schedule|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interstitials|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_interstitials';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Show tabs',
    'weight' => '0',
    'children' => array(
      0 => 'group_interstitials_general',
      1 => 'group_interstitials_admindata',
      2 => 'group_interstitials_content',
      3 => 'group_interstitials_schedule',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_interstitials|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_templates_text_text|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_templates_text_text';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_templates_text';
  $field_group->data = array(
    'label' => 'Brödtext',
    'weight' => '14',
    'children' => array(
      0 => 'field_okv_text',
      1 => 'field_text_color',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Brödtext',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_templates_text_text|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_templates_text_title|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_templates_text_title';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_templates_text';
  $field_group->data = array(
    'label' => 'Rubrik',
    'weight' => '13',
    'children' => array(
      0 => 'field_title',
      1 => 'field_title_color',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Rubrik',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_templates_text_title|node|cinegy_interstitial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_templates_text|node|cinegy_interstitial|form';
  $field_group->group_name = 'group_templates_text';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cinegy_interstitial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_interstitials_content';
  $field_group->data = array(
    'label' => 'Text',
    'weight' => '10',
    'children' => array(
      0 => 'field_background_image',
      1 => 'group_templates_text_title',
      2 => 'group_templates_text_text',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Text',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_templates_text|node|cinegy_interstitial|form'] = $field_group;

  return $export;
}
