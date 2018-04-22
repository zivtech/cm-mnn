<?php

function internet_archive_views_default_views() {  // Add view to list of views to provide.
  $view = new view();
  $view->name = 'internet_archive_transfers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Internet Archive Transfers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Internet Archive Transfers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access internet_archive logs';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '209';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
									 'views_bulk_operations' => 'views_bulk_operations',
									 'tid' => 'tid',
									 'title_1' => 'title_1',
									 'in_path' => 'in_path',
									 'field_name' => 'field_name',
									 'file_location' => 'file_location',
									 'item' => 'item',
									 'status' => 'status',
									 'date_1' => 'date_1',
									 'transfer_initiated' => 'transfer_initiated',
									 );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
								      'views_bulk_operations' => array(
												       'align' => '',
												       'separator' => '',
												       'empty_column' => 0,
												       ),
								      'tid' => array(
										     'sortable' => 1,
										     'default_sort_order' => 'asc',
										     'align' => '',
										     'separator' => '',
										     'empty_column' => 0,
										     ),
								      'title_1' => array(
											 'sortable' => 1,
											 'default_sort_order' => 'asc',
											 'align' => '',
											 'separator' => '',
											 'empty_column' => 0,
											 ),
								      'in_path' => array(
											 'sortable' => 1,
											 'default_sort_order' => 'asc',
											 'align' => '',
											 'separator' => '',
											 'empty_column' => 0,
											 ),
								      'field_name' => array(
											    'sortable' => 1,
											    'default_sort_order' => 'asc',
											    'align' => '',
											    'separator' => '',
											    'empty_column' => 0,
											    ),
								      'file_location' => array(
											       'sortable' => 1,
											       'default_sort_order' => 'asc',
											       'align' => '',
											       'separator' => '',
											       'empty_column' => 0,
											       ),
								      'item' => array(
										      'sortable' => 1,
										      'default_sort_order' => 'asc',
										      'align' => '',
										      'separator' => '',
										      'empty_column' => 0,
										      ),
								      'status' => array(
											'sortable' => 1,
											'default_sort_order' => 'asc',
											'align' => '',
											'separator' => '',
											'empty_column' => 0,
											),
								      'date_1' => array(
											'sortable' => 1,
											'default_sort_order' => 'asc',
											'align' => '',
											'separator' => '',
											'empty_column' => 0,
											),
								      'transfer_initiated' => array(
												    'sortable' => 1,
												    'default_sort_order' => 'asc',
												    'align' => '',
												    'separator' => '',
												    'empty_column' => 0,
												    ),
								      );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Warning! Do not do to many actions at a time to avoid pounding Archive.org';
  $handler->display->display_options['header']['area']['format'] = 'ckeditor';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Relationship: Internet Archive: Node ID */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'internet_archive';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
												  'action::internet_archive_delete_archive_item_action' => array(
																				 'selected' => 1,
																				 'postpone_processing' => 0,
																				 'skip_confirmation' => 0,
																				 'override_label' => 0,
																				 'label' => '',
																				 ),
												  'action::internet_archive_complete_transfer_action' => array(
																			       'selected' => 1,
																			       'postpone_processing' => 0,
																			       'skip_confirmation' => 0,
																			       'override_label' => 0,
																			       'label' => '',
																			       ),
												  );
  /* Field: Internet Archive: Transfer ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Node Title';
  /* Field: Internet Archive: Source Path */
  $handler->display->display_options['fields']['in_path']['id'] = 'in_path';
  $handler->display->display_options['fields']['in_path']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['in_path']['field'] = 'in_path';
  $handler->display->display_options['fields']['in_path']['label'] = 'File Name';
  /* Field: Internet Archive: Field Name */
  $handler->display->display_options['fields']['field_name']['id'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['field_name']['field'] = 'field_name';
  /* Field: Internet Archive: File Location */
  $handler->display->display_options['fields']['file_location']['id'] = 'file_location';
  $handler->display->display_options['fields']['file_location']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['file_location']['field'] = 'file_location';
  /* Field: Internet Archive: Item Identifier */
  $handler->display->display_options['fields']['item']['id'] = 'item';
  $handler->display->display_options['fields']['item']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['item']['field'] = 'item';
  $handler->display->display_options['fields']['item']['label'] = 'Archive Item Name';
  /* Field: Internet Archive: Transfer Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['relationship'] = 'nid';
  /* Field: Internet Archive: Queue Date */
  $handler->display->display_options['fields']['date_1']['id'] = 'date_1';
  $handler->display->display_options['fields']['date_1']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['date_1']['field'] = 'date';
  $handler->display->display_options['fields']['date_1']['date_format'] = 'short';
  $handler->display->display_options['fields']['date_1']['second_date_format'] = 'long';
  /* Field: Internet Archive: Transfer Initiated Date */
  $handler->display->display_options['fields']['transfer_initiated']['id'] = 'transfer_initiated';
  $handler->display->display_options['fields']['transfer_initiated']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['transfer_initiated']['field'] = 'transfer_initiated';
  $handler->display->display_options['fields']['transfer_initiated']['date_format'] = 'short';
  $handler->display->display_options['fields']['transfer_initiated']['second_date_format'] = 'long';
  /* Field: Internet Archive: Queue Attempts */
  $handler->display->display_options['fields']['attempts']['id'] = 'attempts';
  $handler->display->display_options['fields']['attempts']['table'] = 'internet_archive';
  $handler->display->display_options['fields']['attempts']['field'] = 'attempts';
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
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'allwords';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
											     2 => '2',
											     3 => 0,
											     1 => 0,
											     44 => 0,
											     8 => 0,
											     14 => 0,
											     10 => 0,
											     6 => 0,
											     11 => 0,
											     4 => 0,
											     51 => 0,
											     16 => 0,
											     49 => 0,
											     17 => 0,
											     20 => 0,
											     22 => 0,
											     23 => 0,
											     50 => 0,
											     25 => 0,
											     48 => 0,
											     39 => 0,
											     28 => 0,
											     29 => 0,
											     45 => 0,
											     26 => 0,
											     37 => 0,
											     33 => 0,
											     34 => 0,
											     35 => 0,
											     46 => 0,
											     36 => 0,
											     43 => 0,
											     47 => 0,
											     32 => 0,
											     );
  $handler->display->display_options['filters']['title']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['title']['expose']['autocomplete_field'] = 'title_1';
  $handler->display->display_options['filters']['title']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['title']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['title']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Internet Archive: Source Path */
  $handler->display->display_options['filters']['in_path']['id'] = 'in_path';
  $handler->display->display_options['filters']['in_path']['table'] = 'internet_archive';
  $handler->display->display_options['filters']['in_path']['field'] = 'in_path';
  $handler->display->display_options['filters']['in_path']['operator'] = 'allwords';
  $handler->display->display_options['filters']['in_path']['group'] = 1;
  $handler->display->display_options['filters']['in_path']['exposed'] = TRUE;
  $handler->display->display_options['filters']['in_path']['expose']['operator_id'] = 'in_path_op';
  $handler->display->display_options['filters']['in_path']['expose']['label'] = 'File Name';
  $handler->display->display_options['filters']['in_path']['expose']['operator'] = 'in_path_op';
  $handler->display->display_options['filters']['in_path']['expose']['identifier'] = 'in_path';
  $handler->display->display_options['filters']['in_path']['expose']['remember_roles'] = array(
											       2 => '2',
											       3 => 0,
											       1 => 0,
											       44 => 0,
											       8 => 0,
											       14 => 0,
											       10 => 0,
											       6 => 0,
											       11 => 0,
											       4 => 0,
											       51 => 0,
											       16 => 0,
											       49 => 0,
											       17 => 0,
											       20 => 0,
											       22 => 0,
											       23 => 0,
											       50 => 0,
											       25 => 0,
											       48 => 0,
											       39 => 0,
											       28 => 0,
											       29 => 0,
											       45 => 0,
											       26 => 0,
											       37 => 0,
											       33 => 0,
											       34 => 0,
											       35 => 0,
											       46 => 0,
											       36 => 0,
											       43 => 0,
											       47 => 0,
											       32 => 0,
											       );
  $handler->display->display_options['filters']['in_path']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['in_path']['expose']['autocomplete_field'] = 'in_path';
  $handler->display->display_options['filters']['in_path']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['in_path']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['in_path']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Internet Archive: Transfer Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'internet_archive';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  $handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status_1']['expose']['operator_id'] = 'status_1_op';
  $handler->display->display_options['filters']['status_1']['expose']['label'] = 'Transfer Status';
  $handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
  $handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['expose']['remember_roles'] = array(
												2 => '2',
												3 => 0,
												1 => 0,
												44 => 0,
												8 => 0,
												14 => 0,
												10 => 0,
												6 => 0,
												11 => 0,
												4 => 0,
												51 => 0,
												16 => 0,
												49 => 0,
												17 => 0,
												20 => 0,
												22 => 0,
												23 => 0,
												50 => 0,
												25 => 0,
												48 => 0,
												39 => 0,
												28 => 0,
												29 => 0,
												45 => 0,
												26 => 0,
												37 => 0,
												33 => 0,
												34 => 0,
												35 => 0,
												46 => 0,
												36 => 0,
												43 => 0,
												47 => 0,
												32 => 0,
												);
  $handler->display->display_options['filters']['status_1']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['status_1']['expose']['autocomplete_field'] = 'status';
  $handler->display->display_options['filters']['status_1']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['status_1']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['status_1']['expose']['autocomplete_dependent'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/reports/internet_archive/transfers';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Transfers';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;
  
  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;

}