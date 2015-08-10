<?php
/**
 * @file
 * mnn_airing_import.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mnn_airing_import_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}
