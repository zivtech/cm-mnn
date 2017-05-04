<?php

/**
 * @file
 * Drush integration for Internet Archive Module.
 */

/**
 * Implements hook_drush_help().
 */
function internet_archive_download_drush_help($section) {
  switch ($section) {
    case 'drush:ia-download':
      return dt('Execute download of queued files from the Internet Archive');
  }
}

/**
 * Implements hook_queue_drush_command().
 */
function internet_archive_download_drush_command() {
  $items = array();
  $items['ia-download'] = array(
    'callback' => 'internet_archive_download_queue',
    'description' => 'Run Internet Archive Download Drupal queue workers.',
  );
  return $items;
}
