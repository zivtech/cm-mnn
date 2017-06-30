<?php

function internet_archive_mnn_metadata_views_default_views() {
  //FOR EACH VIEW, COPY EXPORT CODE THEN ADD IT TO THIS ARRAY
  $views = array();

  $view = new view();
  $view->name = 'internet_archive_file_harvester';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Internet Archive File Harvester';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
									 'title' => 'title',
									 'field_internet_archive' => 'field_internet_archive',
									 'nid' => 'nid',
									 );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
								      'title' => array(
										       'sortable' => 0,
										       'default_sort_order' => 'asc',
										       'align' => '',
										       'separator' => '',
										       'empty_column' => 0,
										       ),
								      'field_internet_archive' => array(
													'sortable' => 0,
													'default_sort_order' => 'asc',
													'align' => '',
													'separator' => '',
													'empty_column' => 0,
													),
								      'nid' => array(
										     'sortable' => 0,
										     'default_sort_order' => 'asc',
										     'align' => '',
										     'separator' => '',
										     'empty_column' => 0,
										     ),
								      );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Internet Archive */
  $handler->display->display_options['fields']['field_internet_archive']['id'] = 'field_internet_archive';
  $handler->display->display_options['fields']['field_internet_archive']['table'] = 'field_data_field_internet_archive';
  $handler->display->display_options['fields']['field_internet_archive']['field'] = 'field_internet_archive';
  $handler->display->display_options['fields']['field_internet_archive']['label'] = '';
  $handler->display->display_options['fields']['field_internet_archive']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_internet_archive']['click_sort_column'] = 'fid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
									 'cm_show' => 'cm_show',
									 );
  /* Filter criterion: Content: Internet Archive (field_internet_archive:fid) */
  $handler->display->display_options['filters']['field_internet_archive_fid']['id'] = 'field_internet_archive_fid';
  $handler->display->display_options['filters']['field_internet_archive_fid']['table'] = 'field_data_field_internet_archive';
  $handler->display->display_options['filters']['field_internet_archive_fid']['field'] = 'field_internet_archive_fid';
  $handler->display->display_options['filters']['field_internet_archive_fid']['operator'] = '>';
  $handler->display->display_options['filters']['field_internet_archive_fid']['value']['value'] = '0';
  

  // Add view to list of views to provide.
  $views[$view->name] = $view;
  
  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;

}