<?php

function cma_program_import_views_default_views() {
  //FOR EACH VIEW, COPY EXPORT CODE THEN ADD IT TO THIS ARRAY
  $views = array();

  //ROLLBACK AIRINGS VIEW
  $view = new view();
  $view->name = 'program_import_rollback_airings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'cm_airing';
  $view->human_name = 'program_import_rollback_airings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Program Import Rollback Airings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
							       3 => '3',
							       10 => '10',
							       );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_uploader_airingref_node']['id'] = 'reverse_field_uploader_airingref_node';
  $handler->display->display_options['relationships']['reverse_field_uploader_airingref_node']['table'] = 'cm_airing';
  $handler->display->display_options['relationships']['reverse_field_uploader_airingref_node']['field'] = 'reverse_field_uploader_airingref_node';
  $handler->display->display_options['relationships']['reverse_field_uploader_airingref_node']['required'] = TRUE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_overlap_airingconflict']['id'] = 'reverse_field_conflict_airing_overlap_airingconflict';
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_overlap_airingconflict']['table'] = 'cm_airing';
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_overlap_airingconflict']['field'] = 'reverse_field_conflict_airing_overlap_airingconflict';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_original_airingconflict']['id'] = 'reverse_field_conflict_airing_original_airingconflict';
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_original_airingconflict']['table'] = 'cm_airing';
  $handler->display->display_options['relationships']['reverse_field_conflict_airing_original_airingconflict']['field'] = 'reverse_field_conflict_airing_original_airingconflict';
  /* Field: Bulk operations: Airing */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
												  'rules_component::rules_airing_delete' => array(
																		  'selected' => 1,
																		  'postpone_processing' => 0,
																		  'skip_confirmation' => 0,
																		  'override_label' => 0,
																		  'label' => '',
																		  ),
												  );
  /* Field: Airing: Airing ID */
  $handler->display->display_options['fields']['airing_id']['id'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['airing_id']['field'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['airing_id']['separator'] = '';
  /* Field: Airing: Title */
  $handler->display->display_options['fields']['field_airing_title']['id'] = 'field_airing_title';
  $handler->display->display_options['fields']['field_airing_title']['table'] = 'field_data_field_airing_title';
  $handler->display->display_options['fields']['field_airing_title']['field'] = 'field_airing_title';
  $handler->display->display_options['fields']['field_airing_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_airing_title']['alter']['path'] = 'airing/[airing_id]';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Time';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
  print cma_program_import_date_view_field($row);
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Airing: Airing Grid Link */
  $handler->display->display_options['fields']['field_airing_grid_link']['id'] = 'field_airing_grid_link';
  $handler->display->display_options['fields']['field_airing_grid_link']['table'] = 'field_data_field_airing_grid_link';
  $handler->display->display_options['fields']['field_airing_grid_link']['field'] = 'field_airing_grid_link';
  $handler->display->display_options['fields']['field_airing_grid_link']['type'] = 'computed_field_unsanitized';
  /* Field: Airing Conflict: Original Airing */
  $handler->display->display_options['fields']['field_conflict_airing_original']['id'] = 'field_conflict_airing_original';
  $handler->display->display_options['fields']['field_conflict_airing_original']['table'] = 'field_data_field_conflict_airing_original';
  $handler->display->display_options['fields']['field_conflict_airing_original']['field'] = 'field_conflict_airing_original';
  $handler->display->display_options['fields']['field_conflict_airing_original']['relationship'] = 'reverse_field_conflict_airing_overlap_airingconflict';
  $handler->display->display_options['fields']['field_conflict_airing_original']['label'] = 'Overlap';
  $handler->display->display_options['fields']['field_conflict_airing_original']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_conflict_airing_original']['settings'] = array(
												     'link' => 1,
												     );
  /* Field: Airing Conflict: Overlap Airing */
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['id'] = 'field_conflict_airing_overlap';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['table'] = 'field_data_field_conflict_airing_overlap';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['field'] = 'field_conflict_airing_overlap';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['relationship'] = 'reverse_field_conflict_airing_original_airingconflict';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['label'] = 'Overlap';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['empty'] = '[field_conflict_airing_original]';
  $handler->display->display_options['fields']['field_conflict_airing_overlap']['settings'] = array(
												    'link' => 1,
												    );
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'reverse_field_uploader_airingref_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/programming/program_import/rollback_airings';
  
  // Add view to list of views to provide.
  $views[$view->name] = $view;

  //ROLLBACK SHOWS VIEW
  $view = new view();
  $view->name = 'program_import_rollback_shows';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'program_import_rollback_shows';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Program Import Rollback Shows';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
							       3 => '3',
							       10 => '10',
							       );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_uploader_showref_node']['id'] = 'reverse_field_uploader_showref_node';
  $handler->display->display_options['relationships']['reverse_field_uploader_showref_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_uploader_showref_node']['field'] = 'reverse_field_uploader_showref_node';
  $handler->display->display_options['relationships']['reverse_field_uploader_showref_node']['required'] = TRUE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Show Rollback';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
												  'action::views_bulk_operations_delete_item' => array(
																		       'selected' => 1,
																		       'postpone_processing' => 0,
																		       'skip_confirmation' => 0,
																		       'override_label' => 0,
																		       'label' => '',
																		       ),
												  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: External Episode Number */
  $handler->display->display_options['fields']['field_external_episode_number']['id'] = 'field_external_episode_number';
  $handler->display->display_options['fields']['field_external_episode_number']['table'] = 'field_data_field_external_episode_number';
  $handler->display->display_options['fields']['field_external_episode_number']['field'] = 'field_external_episode_number';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'reverse_field_uploader_showref_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
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

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'program-import-rollback-shows';
  
  // Add view to list of views to provide.
  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'program_import_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Program Import Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Program Import Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Import';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Import Date Range';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
  print cma_program_import_airing_range_view_field($row);
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
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
									 'program_import' => 'program_import',
									 );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '>=';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Min Run Date';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created_1']['id'] = 'created_1';
  $handler->display->display_options['filters']['created_1']['table'] = 'node';
  $handler->display->display_options['filters']['created_1']['field'] = 'created';
  $handler->display->display_options['filters']['created_1']['operator'] = '<=';
  $handler->display->display_options['filters']['created_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created_1']['expose']['operator_id'] = 'created_1_op';
  $handler->display->display_options['filters']['created_1']['expose']['label'] = 'Max Run Date';
  $handler->display->display_options['filters']['created_1']['expose']['operator'] = 'created_1_op';
  $handler->display->display_options['filters']['created_1']['expose']['identifier'] = 'created_1';
  $handler->display->display_options['filters']['created_1']['expose']['remember_roles'] = array(
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

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/programming/program-import-search';
  
  // Add view to list of views to provide.
  $views[$view->name] = $view;
  
  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;

}