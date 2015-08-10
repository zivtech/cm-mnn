<?php
/**
 * @file
 * cinegy_interstitial.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function cinegy_interstitial_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_cinegy_interstitial';
  $strongarm->value = 0;
  $export['comment_anonymous_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_cinegy_interstitial';
  $strongarm->value = '1';
  $export['comment_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_cinegy_interstitial';
  $strongarm->value = 1;
  $export['comment_default_mode_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_cinegy_interstitial';
  $strongarm->value = '50';
  $export['comment_default_per_page_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_cinegy_interstitial';
  $strongarm->value = 1;
  $export['comment_form_location_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_cinegy_interstitial';
  $strongarm->value = '1';
  $export['comment_preview_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_cinegy_interstitial';
  $strongarm->value = 1;
  $export['comment_subject_field_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__cinegy_interstitial';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_cinegy_interstitial';
  $strongarm->value = '0';
  $export['language_content_type_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_cinegy_interstitial';
  $strongarm->value = array();
  $export['menu_options_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_cinegy_interstitial';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_cinegy_interstitial';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_cinegy_interstitial';
  $strongarm->value = '1';
  $export['node_preview_cinegy_interstitial'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_cinegy_interstitial';
  $strongarm->value = 0;
  $export['node_submitted_cinegy_interstitial'] = $strongarm;

  return $export;
}
