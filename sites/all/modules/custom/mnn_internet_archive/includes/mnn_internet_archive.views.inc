<?php

function mnn_internet_archive_views_default_views() {
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
  /* Field: Content: Internet Archive Remote */
  $handler->display->display_options['fields']['field_internet_archive_remote']['id'] = 'field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['table'] = 'field_data_field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['field'] = 'field_internet_archive_remote';
  /* Field: Content: Internet Archive Local */
  $handler->display->display_options['fields']['field_internet_archive_local']['id'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['table'] = 'field_data_field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['field'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['click_sort_column'] = 'fid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['groups'] = array(
									1 => 'AND',
									2 => 'OR',
									);
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
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Internet Archive Local (field_internet_archive_local:fid) */
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['id'] = 'field_internet_archive_local_fid';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['table'] = 'field_data_field_internet_archive_local';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['field'] = 'field_internet_archive_local_fid';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['operator'] = '>';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['value']['value'] = '0';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['group'] = 2;
  /* Filter criterion: Content: Internet Archive Remote (field_internet_archive_remote) */
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['id'] = 'field_internet_archive_remote_value';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['table'] = 'field_data_field_internet_archive_remote';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['field'] = 'field_internet_archive_remote_value';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['group'] = 2;
  
  // Add view to list of views to provide.
  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'internet_archive_video_embed';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Internet Archive Video Embed';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Internet Archive: Recent Uploads';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Internet Archive Local */
  $handler->display->display_options['fields']['field_internet_archive_local']['id'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['table'] = 'field_data_field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['field'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['label'] = '';
  $handler->display->display_options['fields']['field_internet_archive_local']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_internet_archive_local']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_local']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_local']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_internet_archive_local']['type'] = 'internet_archive_video_embed';
  $handler->display->display_options['fields']['field_internet_archive_local']['settings'] = array(
												   'archive_links' => 'hide',
												   'archive_embed_width' => '470',
												   'archive_embed_height' => '371',
												   );
  /* Field: Content: Internet Archive Remote */
  $handler->display->display_options['fields']['field_internet_archive_remote']['id'] = 'field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['table'] = 'field_data_field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['field'] = 'field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['label'] = '';
  $handler->display->display_options['fields']['field_internet_archive_remote']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_internet_archive_remote']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_remote']['type'] = 'internet_archive_video_embed';
  $handler->display->display_options['fields']['field_internet_archive_remote']['settings'] = array(
												    'archive_links' => 'hide',
												    'archive_embed_width' => '470',
												    'archive_embed_height' => '371',
												    );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['groups'] = array(
									1 => 'AND',
									2 => 'OR',
									);
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Internet Archive Remote (field_internet_archive_remote) */
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['id'] = 'field_internet_archive_remote_value';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['table'] = 'field_data_field_internet_archive_remote';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['field'] = 'field_internet_archive_remote_value';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_internet_archive_remote_value']['group'] = 2;
  /* Filter criterion: Content: Internet Archive Local (field_internet_archive_local:fid) */
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['id'] = 'field_internet_archive_local_fid';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['table'] = 'field_data_field_internet_archive_local';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['field'] = 'field_internet_archive_local_fid';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['operator'] = '>';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['value']['value'] = '0';
  $handler->display->display_options['filters']['field_internet_archive_local_fid']['group'] = 2;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'internet-archive/recent-videos';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = '<none>';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Internet Archive Local */
  $handler->display->display_options['fields']['field_internet_archive_local']['id'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['table'] = 'field_data_field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['field'] = 'field_internet_archive_local';
  $handler->display->display_options['fields']['field_internet_archive_local']['label'] = '';
  $handler->display->display_options['fields']['field_internet_archive_local']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_internet_archive_local']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_local']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_local']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_internet_archive_local']['type'] = 'internet_archive_video_embed';
  $handler->display->display_options['fields']['field_internet_archive_local']['settings'] = array(
												   'archive_links' => 'hide',
												   'archive_embed_width' => '470',
												   'archive_embed_height' => '371',
												   );
  /* Field: Content: Internet Archive Remote */
  $handler->display->display_options['fields']['field_internet_archive_remote']['id'] = 'field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['table'] = 'field_data_field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['field'] = 'field_internet_archive_remote';
  $handler->display->display_options['fields']['field_internet_archive_remote']['label'] = '';
  $handler->display->display_options['fields']['field_internet_archive_remote']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_internet_archive_remote']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_internet_archive_remote']['type'] = 'internet_archive_video_embed';
  $handler->display->display_options['fields']['field_internet_archive_remote']['settings'] = array(
												    'archive_links' => 'hide',
												    'archive_embed_width' => '470',
												    'archive_embed_height' => '371',
												    );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  
  // Add view to list of views to provide.
  $views[$view->name] = $view;
  
  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;

}