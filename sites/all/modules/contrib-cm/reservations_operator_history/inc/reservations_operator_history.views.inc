<?php

function reservations_operator_history_views_default_views() {
  $view = new view();
  $view->name = 'reservation_operator_actions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'reservations_operator_history';
  $view->human_name = 'Reservation: Operator Actions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reservation: Operator Actions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Reservations Operator History: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: Reservations Operator History: Beginning Reservation Status */
  $handler->display->display_options['fields']['beginning_status']['id'] = 'beginning_status';
  $handler->display->display_options['fields']['beginning_status']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['beginning_status']['field'] = 'beginning_status';
  $handler->display->display_options['fields']['beginning_status']['label'] = '';
  $handler->display->display_options['fields']['beginning_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['beginning_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['beginning_status']['separator'] = '';
  /* Field: Reservations Operator History: End Reservation Status */
  $handler->display->display_options['fields']['end_status']['id'] = 'end_status';
  $handler->display->display_options['fields']['end_status']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['end_status']['field'] = 'end_status';
  $handler->display->display_options['fields']['end_status']['label'] = '';
  $handler->display->display_options['fields']['end_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['end_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['end_status']['separator'] = '';
  /* Field: Reservations Operator History: Reservation nid */
  $handler->display->display_options['fields']['reservation_id']['id'] = 'reservation_id';
  $handler->display->display_options['fields']['reservation_id']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['reservation_id']['field'] = 'reservation_id';
  $handler->display->display_options['fields']['reservation_id']['label'] = '';
  $handler->display->display_options['fields']['reservation_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['reservation_id']['element_label_colon'] = FALSE;
  /* Field: Reservations Operator History: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'medium';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: Reservations Operator History: UID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'reservations_operator_history';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['separator'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Reservation';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php']['php_output'] = '<?php
print reservations_operator_history_reservation_field($row);
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'Operator';
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_1']['php_output'] = '<?php
print reservations_operator_history_operator_field($row);
?>
';
$handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
/* Field: Global: PHP */
$handler->display->display_options['fields']['php_3']['id'] = 'php_3';
$handler->display->display_options['fields']['php_3']['table'] = 'views';
$handler->display->display_options['fields']['php_3']['field'] = 'php';
$handler->display->display_options['fields']['php_3']['label'] = 'Beginning Status';
$handler->display->display_options['fields']['php_3']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_3']['php_output'] = '<?php
print reservations_operator_history_status_field($row, \'beginning\');
?>
';
$handler->display->display_options['fields']['php_3']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_3']['php_click_sortable'] = '';
/* Field: Global: PHP */
$handler->display->display_options['fields']['php_2']['id'] = 'php_2';
$handler->display->display_options['fields']['php_2']['table'] = 'views';
$handler->display->display_options['fields']['php_2']['field'] = 'php';
$handler->display->display_options['fields']['php_2']['label'] = 'Ending Status';
$handler->display->display_options['fields']['php_2']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_2']['php_output'] = '<?php
print reservations_operator_history_status_field($row, \'ending\');
?>
';
$handler->display->display_options['fields']['php_2']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_2']['php_click_sortable'] = '';
/* Sort criterion: Reservations Operator History: Timestamp */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'reservations_operator_history';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'reservation_id';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/reservation-operator-actions';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Reservations Operator History: Operatory History Reservation Relationship */
$handler->display->display_options['relationships']['reservation_id']['id'] = 'reservation_id';
$handler->display->display_options['relationships']['reservation_id']['table'] = 'reservations_operator_history';
$handler->display->display_options['relationships']['reservation_id']['field'] = 'reservation_id';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Reservations Operator History: ID */
$handler->display->display_options['fields']['id']['id'] = 'id';
$handler->display->display_options['fields']['id']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['id']['field'] = 'id';
$handler->display->display_options['fields']['id']['label'] = '';
$handler->display->display_options['fields']['id']['exclude'] = TRUE;
$handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['id']['separator'] = '';
/* Field: Reservations Operator History: Beginning Reservation Status */
$handler->display->display_options['fields']['beginning_status']['id'] = 'beginning_status';
$handler->display->display_options['fields']['beginning_status']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['beginning_status']['field'] = 'beginning_status';
$handler->display->display_options['fields']['beginning_status']['label'] = '';
$handler->display->display_options['fields']['beginning_status']['exclude'] = TRUE;
$handler->display->display_options['fields']['beginning_status']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['beginning_status']['separator'] = '';
/* Field: Reservations Operator History: End Reservation Status */
$handler->display->display_options['fields']['end_status']['id'] = 'end_status';
$handler->display->display_options['fields']['end_status']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['end_status']['field'] = 'end_status';
$handler->display->display_options['fields']['end_status']['label'] = '';
$handler->display->display_options['fields']['end_status']['exclude'] = TRUE;
$handler->display->display_options['fields']['end_status']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['end_status']['separator'] = '';
/* Field: Reservations Operator History: Reservation nid */
$handler->display->display_options['fields']['reservation_id']['id'] = 'reservation_id';
$handler->display->display_options['fields']['reservation_id']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['reservation_id']['field'] = 'reservation_id';
$handler->display->display_options['fields']['reservation_id']['label'] = '';
$handler->display->display_options['fields']['reservation_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['reservation_id']['element_label_colon'] = FALSE;
/* Field: Reservations Operator History: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'medium';
$handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
/* Field: Reservations Operator History: UID */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'reservations_operator_history';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = '';
$handler->display->display_options['fields']['uid']['exclude'] = TRUE;
$handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['uid']['separator'] = '';
/* Field: Global: PHP */
$handler->display->display_options['fields']['php_1']['id'] = 'php_1';
$handler->display->display_options['fields']['php_1']['table'] = 'views';
$handler->display->display_options['fields']['php_1']['field'] = 'php';
$handler->display->display_options['fields']['php_1']['label'] = 'Operator';
$handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_1']['php_output'] = '<?php
print reservations_operator_history_operator_field($row);
?>
';
$handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
/* Field: Global: PHP */
$handler->display->display_options['fields']['php_3']['id'] = 'php_3';
$handler->display->display_options['fields']['php_3']['table'] = 'views';
$handler->display->display_options['fields']['php_3']['field'] = 'php';
$handler->display->display_options['fields']['php_3']['label'] = 'Beginning Status';
$handler->display->display_options['fields']['php_3']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_3']['php_output'] = '<?php
print reservations_operator_history_status_field($row, \'beginning\');
?>
';
$handler->display->display_options['fields']['php_3']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_3']['php_click_sortable'] = '';
/* Field: Global: PHP */
$handler->display->display_options['fields']['php_2']['id'] = 'php_2';
$handler->display->display_options['fields']['php_2']['table'] = 'views';
$handler->display->display_options['fields']['php_2']['field'] = 'php';
$handler->display->display_options['fields']['php_2']['label'] = 'Ending Status';
$handler->display->display_options['fields']['php_2']['use_php_setup'] = 0;
$handler->display->display_options['fields']['php_2']['php_output'] = '<?php
print reservations_operator_history_status_field($row, \'ending\');
?>
';
$handler->display->display_options['fields']['php_2']['use_php_click_sortable'] = '0';
$handler->display->display_options['fields']['php_2']['php_click_sortable'] = '';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'reservation_id';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
  
}