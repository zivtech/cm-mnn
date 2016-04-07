<?php

function reservations_report_views_default_views() {
  $view = new view();
  $view->name = 'reservations_reports';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Reservations: Reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reservation Usage Counts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage reservations';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['content'] = '<?php
  print reservations_report_usage_counts_report_header();
?>';
$handler->display->display_options['header']['area']['format'] = 'php_code';
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Reservations: Reserved item nid */
$handler->display->display_options['relationships']['item_nid']['id'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['table'] = 'reservations_reservation_detail';
$handler->display->display_options['relationships']['item_nid']['field'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['required'] = TRUE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'User Name';
/* Field: Content: Reservation Dates */
$handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
$handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
$handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
$handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
											    'format_type' => 'short',
											    'fromto' => 'both',
											    'multiple_number' => '',
											    'multiple_from' => '',
											    'multiple_to' => '',
											    'show_repeat_rule' => 'show',
											    );
/* Field: Reservations: Reservation status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
/* Field: Content: Title */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'node';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['relationship'] = 'item_nid';
$handler->display->display_options['fields']['title_1']['label'] = 'Reserved Item';
/* Field: Reservations: Rate per hour -- default */
$handler->display->display_options['fields']['rate_per_hour']['id'] = 'rate_per_hour';
$handler->display->display_options['fields']['rate_per_hour']['table'] = 'reservations_node_type';
$handler->display->display_options['fields']['rate_per_hour']['field'] = 'rate_per_hour';
$handler->display->display_options['fields']['rate_per_hour']['relationship'] = 'item_nid';
$handler->display->display_options['fields']['rate_per_hour']['label'] = 'Rate per hour';
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'item_nid';
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
								       'reservations_reservation' => 'reservations_reservation',
								       );
/* Filter criterion: Content: Reservation Dates -  start date (field_reservations_date) */
$handler->display->display_options['filters']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
$handler->display->display_options['filters']['field_reservations_date_value']['table'] = 'field_data_field_reservations_date';
$handler->display->display_options['filters']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
$handler->display->display_options['filters']['field_reservations_date_value']['operator'] = 'between';
$handler->display->display_options['filters']['field_reservations_date_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_reservations_date_value']['expose']['operator_id'] = 'field_reservations_date_value_op';
$handler->display->display_options['filters']['field_reservations_date_value']['expose']['label'] = 'Reservation Dates';
$handler->display->display_options['filters']['field_reservations_date_value']['expose']['operator'] = 'field_reservations_date_value_op';
$handler->display->display_options['filters']['field_reservations_date_value']['expose']['identifier'] = 'field_reservations_date_value';
$handler->display->display_options['filters']['field_reservations_date_value']['expose']['remember_roles'] = array(
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
														   16 => 0,
														   17 => 0,
														   20 => 0,
														   22 => 0,
														   23 => 0,
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
$handler->display->display_options['filters']['field_reservations_date_value']['default_date'] = 'now -1 week';
$handler->display->display_options['filters']['field_reservations_date_value']['default_to_date'] = 'now';
/* Filter criterion: Reservations: Reservation status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'reservations_reservation';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['value'] = array(
									   4 => '4',
									   );
$handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['status_1']['expose']['operator_id'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Reservation status';
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['multiple'] = TRUE;
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
											      16 => 0,
											      17 => 0,
											      20 => 0,
											      22 => 0,
											      23 => 0,
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
/* Filter criterion: Reservations: Reservations type */
$handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
$handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
$handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
$handler->display->display_options['filters']['type_setting']['relationship'] = 'item_nid';
$handler->display->display_options['filters']['type_setting']['exposed'] = TRUE;
$handler->display->display_options['filters']['type_setting']['expose']['operator_id'] = 'type_setting_op';
$handler->display->display_options['filters']['type_setting']['expose']['label'] = 'Reservation Type';
$handler->display->display_options['filters']['type_setting']['expose']['operator'] = 'type_setting_op';
$handler->display->display_options['filters']['type_setting']['expose']['identifier'] = 'type_setting';
$handler->display->display_options['filters']['type_setting']['expose']['remember_roles'] = array(
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
												  16 => 0,
												  17 => 0,
												  20 => 0,
												  22 => 0,
												  23 => 0,
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
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type_1']['id'] = 'type_1';
$handler->display->display_options['filters']['type_1']['table'] = 'node';
$handler->display->display_options['filters']['type_1']['field'] = 'type';
$handler->display->display_options['filters']['type_1']['relationship'] = 'item_nid';
$handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
$handler->display->display_options['filters']['type_1']['expose']['label'] = 'Reservable Item type';
$handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
$handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
$handler->display->display_options['filters']['type_1']['expose']['remember_roles'] = array(
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
											    16 => 0,
											    17 => 0,
											    20 => 0,
											    22 => 0,
											    23 => 0,
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

/* Display: Usage Counts */
$handler = $view->new_display('page', 'Usage Counts', 'page');
$handler->display->display_options['path'] = 'admin/reservations/manage/reports/usage-counts';

/* Display: Hours Breakdown */
$handler = $view->new_display('page', 'Hours Breakdown', 'page_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Reservation Breakdown by Hours';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['content'] = '<?php
  print reservations_reports_hourly_breakdown_report_header();
?>';
$handler->display->display_options['header']['area']['format'] = 'php_code';
$handler->display->display_options['path'] = 'admin/reservations/manage/reports/hours-breakdown';


  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.
  // At the end, return array of default views.             
  return $views;
}
