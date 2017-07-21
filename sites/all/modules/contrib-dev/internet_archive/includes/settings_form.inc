<?php

/**
 * Loads the general settings form.
 *
 * @see internet_archive_admin_form()
 */
function internet_archive_settings_home() {
  return drupal_get_form('internet_archive_admin_form');
}

/**
 * Form builder for the internet archive site settings form.
 *
 * @ingroup forms
 */
function internet_archive_admin_form($form) {
  $form['internet_archive_help'] =
    array(
	  '#value' => '<p><strong>' .
	  internet_archive_help_links('using-internet-archive', 'link') .
	  '</strong></p>',
  );

  $form['internet_archive_account'] =
    array(
	  '#type' => 'fieldset',
	  '#element_validate' => array('internet_archive_admin_validate'),
	  '#title' => 'Archive.org Account Configuration',
	  '#description' =>
	  t('Global settings for Archive.org S3 configurations.'),
  );
  
  $default_value =
    variable_get('internet_archive_default_server_url',
		 "http://s3.us.archive.org/");
    
  $form['internet_archive_account']['internet_archive_default_server_url'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('Archive.org S3 URL'),
	  '#default_value' => $default_value,
	  '#description' =>
	  t("URL to send to archive. You probably don't need to change this."),
	  '#required' => TRUE,
	  );
  
  $form['internet_archive_account']['internet_archive_key'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('Archive.org S3 Access Key'),
	  '#default_value' => variable_get('internet_archive_key', ""),
	  '#description' => t("You can obtain one at: ") .
	  l('http://www.archive.org/account/s3.php',
	    'http://www.archive.org/account/s3.php'),
	  '#required' => TRUE,
	  );
  
  $form['internet_archive_account']['internet_archive_skey'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('Archive.org S3 Secret Key'),
	  '#default_value' => variable_get('internet_archive_skey', ""),
	  '#description' => t("You can obtain one at: ") .
	  l('http://www.archive.org/account/s3.php',
	    'http://www.archive.org/account/s3.php'),
	  '#required' => TRUE,
	  );

  $form['internet_archive_data'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => 'Basic item and metadata options',
	  '#description' =>
	  t('Basic item and metadata options. More advanced configurations '.
	    'can override these settings in a custom module via '.
	    'hook_internet_archive_metadata.'),
	  );

  $form['internet_archive_data']['internet_archive_update'] = array(
    '#type' => 'checkbox',
    '#title' =>
    t('Update metadata on archive.org for already transferred files when '.
      'nodes are updated?'),
    '#default_value' => variable_get('internet_archive_update', FALSE),
    '#description' =>
    t("If this option is checked, all item metadata will be resent to ".
      "archive.org when their associated nodes are updated."),
  );
  $default_value =str_replace(' ','_', variable_get('site_name',t('My Site')));
  $default_value = variable_get('internet_archive_bucket', $default_value);

  $form['internet_archive_data']['internet_archive_bucket'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('Default Archive.org Item to store files in'),
	  '#default_value' => $default_value,
	  '#description' =>
	  t("Name of the Archive.org item that files will be placed into, ".
	    "note this has to be unique. This can be overridden by a , ".
	    "configuration but this is the default value and used to test ".
	    "your connection."),
	  '#required' => TRUE,
	  );
  $form['internet_archive_data']['internet_archive_bucket_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use node title for item name when available?'),
    '#default_value' => variable_get('internet_archive_bucket_title', FALSE),
    '#description' =>
    t("This provides a simple alternative to storing files in the default ".
      "item above. Checking this box will result in all transferred files ".
      "being stored in a item/bucket based on their source node title."),
  );
  $form['internet_archive_data']['internet_archive_bucket_body'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use node body for item description when available?'),
    '#default_value' => variable_get('internet_archive_bucket_body', FALSE),
    '#description' =>
    t("If this box is checked, when a node body is available it will be sent ".
      "along as the description for the item on archive.org"),
  );

  $default_value =
    variable_get('internet_archive_collection', "test_collection");
  $description =
    t("Archive.org is broken into many collections. For initial testing ".
      "this defaults to the test collection where files are automatically ".
      "deleted after 30 days. Once you are done testing, you should change ".
      "this to an appropriate permanent collection. You can specify ".
      "multiple collections separated by commas but make sure to put them ".
      "in order of most specific to most general. Note that if you are ".
      "using the om_metadata submodule you do not need to add additional ".
      "collections like openmediaproject or community_media here as the ".
      "submodule handles that automatically") . '<br /><strong>' .
    t("Example: denveropenmedia") . '</strong>';

  $form['internet_archive_data']['internet_archive_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Default collection to store files to'),
    '#default_value' => $default_value,
    '#description' => $description,
  );
  $form['internet_archive_data']['internet_archive_derivatives'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of expected derivatives'),
    '#default_value' => variable_get('internet_archive_derivatives', 3),
    '#description' =>
    t("For a video file, archive.org typically creates 3 derivatives in ".
      "addition to the original file (GIF, OGG, MP4). For audio, there is ".
      "typically 1 derivation (OGG). The module uses this value in order to ".
      "discern when your transfer has finished the derivation process, once ".
      "the number of derivatives reported is equal to or greater than the ".
      "value entered here, the module will update the file status to ".
      "derived and it will start displaying in embeds."),
  );

  $form['internet_archive_data']['internet_archive_mediatype'] = array(
    '#type' => 'select',
    '#title' => t('Default filetype to store files as'),
    '#default_value' => variable_get('internet_archive_mediatype', 'movies'),
    '#options' => array(
      'audio' => 'Audio',
      'education' => 'Education',
      'image' => 'Images',
      'software' => 'Software',
      'movies' => 'Video',
    ),
    '#multiple' => FALSE,
    '#description' =>
    t("This type applies in situations where the system is not provided ".
      "with a media type from hook_internet_archive_metadata, and is not ".
      "able to defer the type from the file itself."),
  );
  
  $description = '<div>' .
    t('Select any fields below that you would like to use to transfer '.
      'files to Archive.org. File, text and embedded media fields are '.
      'supported.') . '<strong>' .
    t(' All fields selected must contain accessible path information as '.
      'their value. Currently this only works with "file" and "text" fields.'.
      'If you are storing path data for files on a remote server, DO NOT '.
      'select the field here. Please enable and configure the Internet '.
      'Archive Remote submodule.') .
    '</strong>';
    
  $form['internet_archive']['internet_archive_field_support'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => 'Field Integration',
	  '#description' => $description,
	  );

  $form['internet_archive']['internet_archive_field_support']
    ['internet_archive_fields'] =
    array(
	  '#type' => 'select',
	  '#title' => t('Include the following fields as file sources'),
	  '#default_value' => variable_get('internet_archive_fields', FALSE),
	  '#options' => internet_archive_field_select_options(),
	  '#multiple' => TRUE,
	  );

  $default_value =
    variable_get('internet_archive_fields_list_display', TRUE);
  
  $form['internet_archive']['internet_archive_field_support']
    ['internet_archive_fields_list_display'] =
    array(
	  '#type' => 'checkbox',
	  '#title' =>
	  t('Display archive.org file information when displaying '.
	    'selected fields on nodes?'),
	  '#default_value' => $default_value,
	  );

  $default_value = variable_get('internet_archive_fields_add_inline', TRUE);
  $form['internet_archive']['internet_archive_field_support']
    ['internet_archive_fields_add_inline'] =
    array(
	  '#type' => 'checkbox',
	  '#title' =>
	  t('Offer "add to Archive.org" link when displaying selected '.
	    'fields on nodes?'),
	  '#default_value' => $default_value,
	  );
  

  $form['internet_archive']['internet_archive_report_settings'] =
    array(
	  '#type' => 'fieldset',
	  '#title' => 'Report Settings',
	  '#description' => '',
	  );

  $form['internet_archive']['internet_archive_report_settings']
    ['internet_archive_results_per_page'] =
    array(
	  '#type' => 'textfield',
	  '#title' => t('Number of results per report page'),
	  '#default_value' =>
	  variable_get('internet_archive_results_per_page', 100),
	  '#description' =>
	  t('The number of results that appear on the '.
	    'Internet Archive Log report'),
	  );

  $default_value =
    variable_get('internet_archive_show_only_errors_and_warnings', FALSE);
  
  $form['internet_archive']['internet_archive_report_settings']
    ['internet_archive_show_only_errors_and_warnings'] =
    array(
	  '#type' => 'checkbox',
	  '#title' =>
	  t('Should the "System Messages" tab on the Internet Archive Log '.
	    'show only Errors and Warnings?'),
	  '#default_value' => $default_value,
	  );
  
  $form['internet_archive']['internet_archive_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Debug Mode'),
    '#default_value' => variable_get('internet_archive_debug', FALSE),
    '#description' =>
    t("Enables verbose watchdog messages for debug purposes."),
  );

  return system_settings_form($form);
}

