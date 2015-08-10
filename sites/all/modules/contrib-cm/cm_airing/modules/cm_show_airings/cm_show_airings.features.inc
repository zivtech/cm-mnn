<?php
/**
 * @file
 * cm_show_airings.features.inc
 */

/**
 * Implements hook_views_api().
 */
function cm_show_airings_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
