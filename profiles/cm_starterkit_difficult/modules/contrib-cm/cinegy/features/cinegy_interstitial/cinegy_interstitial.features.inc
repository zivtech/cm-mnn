<?php
/**
 * @file
 * cinegy_interstitial.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cinegy_interstitial_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function cinegy_interstitial_node_info() {
  $items = array(
    'cinegy_interstitial' => array(
      'name' => t('Interstitial'),
      'base' => 'node_content',
      'description' => t('For storing interstitials, messages that are displayed in a bulletin board format between scheduled TV shows.'),
      'has_title' => '1',
      'title_label' => t('Namn för meddelandet'),
      'help' => '',
    ),
  );
  return $items;
}
