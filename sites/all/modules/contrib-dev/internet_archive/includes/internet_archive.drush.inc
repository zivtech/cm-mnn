<?php

/**
 * @file
 * Drush integration for Internet Archive Module.
 */

/**
 * Implements hook_drush_help().
 */
function internet_archive_drush_help($section) {
  switch ($section) {
    case 'drush:ia-transfer':
      return dt('Execute transfer of queued files to the Internet Archive');
  }
}

/**
 * Implements hook_queue_drush_command().
 */
function internet_archive_drush_command() {
  $items = array();
  $items['ia-transfer'] = array(
    'callback' => 'internet_archive_transfer_queue',
    'description' => 'Run Internet Archive Drupal queue workers.',
  );
  return $items;
}
