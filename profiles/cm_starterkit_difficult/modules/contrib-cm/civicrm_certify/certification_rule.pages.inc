<?php

/**
 * 'Certification rule' view callback.
 */
function certification_rule_view($certification_rule, $view_mode = "full") {
  // Our entity type, for convenience.
  $entity_type = 'certification_rule';
  // Start setting up the content.
  $certification_rule->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($certification_rule->cert_rule_id => $certification_rule),
    $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($certification_rule->cert_rule_id => $certification_rule));
  // Now field_attach_view() generates the content for the fields.
  $certification_rule->content += field_attach_view($entity_type, $certification_rule, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $certification_rule->content['cert_rule_group'] = array(
    '#type' => 'item',
    '#title' => t('CiviGroup'),
    '#markup' => civicrm_certify_group_display($certification_rule->cert_rule_group),

  );
  $certification_rule->content['cert_rule_status'] = array(
    '#type' => 'item',
    '#title' => t('Participant status'),
    '#markup' => civicrm_certify_paricipant_status_display($certification_rule->cert_rule_status),
  );
  $certification_rule->content['cert_rule_event_type'] = array(
    '#type' => 'item',
    '#title' => t('CiviEvent type'),
    '#markup' => civicrm_certify_event_type_display($certification_rule->cert_rule_event_type, "text"),
  );
  $certification_rule->content['cert_rule_name'] = array(
    '#type' => 'item',
    '#title' => t('Certification name'),
    '#markup' => $certification_rule->cert_rule_name,
  );

  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language ;
  $langcode = $language->language ;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $certification_rule, $entity_type, $view_mode,
    $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('certification_rule_view', 'entity_view'),
    $certification_rule->content, $entity_type);

  // And finally return the content.
  return $certification_rule->content;

  //drupal_set_title(entity_label('certification_rule', $certification_rule));
  //return entity_view('certification_rule', array(entity_id('certification_rule', $certification_rule) => $certification_rule), 'full');

}

/**
 * Provide view of certification_rules that appears as a content tab like comments
 */
function certification_rule_content() {
  // we are going to output the results in a table with a nice header
  $header = array(
    array('data' => t('ID'), 'field' => 'cert_rule_id', 'sort' => 'desc'),
    array('data' => t('Title')),
    array('data' => t('Status')),
    array('data' => t('CiviEvent Type')),
    array('data' => t('CiviGroup')),
    array('data' => t('Operations')),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'certification_rule')
    ->entityCondition('entity_type', 'certification_rule')
    ->pager(25)
    ->entityOrderBy('entity_id', 'DESC')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();

  $rows = array();
  if (isset($result['certification_rule'])) {
    $cert_rule_ids = array_keys($result['certification_rule']);
    $cert_rules = entity_load('certification_rule', $cert_rule_ids);

    foreach ( $cert_rules as $entity ) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          //'title' => t("<a href='/certification-rule/{$entity->cert_rule_id}'>$entity->cert_rule_name</a>"),
          'cert_rule_id' => $entity->cert_rule_id,
          'title' => l($entity->cert_rule_name, 'certification-rule/' . $entity->cert_rule_id),
          'status' => civicrm_certify_paricipant_status_display($entity->cert_rule_status),
          'civievent' => civicrm_certify_event_type_display($entity->cert_rule_event_type, "text"),
          'civigroup' => civicrm_certify_group_display($entity->cert_rule_group),

          'Operations' => "<ul class='links inline'>
                            <li class='edit first'><a href='/certification-rule/{$entity->cert_rule_id}/view'>view</a></li>
                            <li class=''><a href='/certification-rule/{$entity->cert_rule_id}/edit'>edit</a></li>
                            <li class='edit last'><a href='/certification-rule/{$entity->cert_rule_id}/sync'>sync</a></li>
                          </ul>",
        ),
      );
    }
  }
  // create a link to add a certification rule
  $build['cert_rule_link'] = array(
    '#theme' => 'link',
    '#text' => '+ Add Certification Rule',
    '#path' => 'certification-rule/add',
    '#options' => array(
      'attributes' => array(
        'class' => array('action-links'),
      ),
      'html' => FALSE,
    ),
    '#suffix' => '<br/>&nbsp;',

   );

  // Create a render array ($build) which will be themed as a table with a
  // pager.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no certification rules found in the db'),
  );

  // attach the pager theme
  $build['pager_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Certification Type edit (new) form.
 */
function certification_rule_form($form, &$form_state, $certification_rule) {
  $form_state['certification_rule'] = $certification_rule;

  $form['cert_rule_id'] = array(
    '#type' => 'hidden',
    '#title' => t('cert_rule_id'),
    '#default_value' => $certification_rule->cert_rule_id,
  );
  $form['cert_rule_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Certification Name'),
    '#default_value' => $certification_rule->cert_rule_name,
    '#required' => TRUE,
    '#description' => t('The name of the certification. This will appear in the user profile as the certification name.'),
  );
  $form['cert_rule_event_type'] = array(
    '#type' => 'select',
    '#title' => t('Event type'),
    '#default_value' => $certification_rule->cert_rule_event_type,
    '#required' => TRUE,
    '#options' => field_cert_rule_civievent_type_values(),
    '#description' => t("The CiviEvent type."),
  );
  $form['cert_rule_status'] = array(
    '#type' => 'select',
    '#title' => t('Event status'),
    '#default_value' => $certification_rule->cert_rule_status,
    '#required' => TRUE,
    '#options' => field_cert_rule_participant_status_values(),
    '#description' => t("The participant status that triggers the certification."),
  );
  $form['cert_rule_group'] = array(
    '#type' => 'select',
    '#title' => t('CiviCRM Group'),
    '#default_value' => $certification_rule->cert_rule_group,
    '#required' => TRUE,
    '#options' => field_cert_rule_civigroup_values(),
    '#description' => t("The group to assign to the user during certification."),
  );

  field_attach_form('certification_rule', $certification_rule, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Certification Type'),
    '#submit' => $submit + array('certification_rule_form_submit'),
  );

  // Show Delete button if we edit certification_rule.
  $certification_rule_id = entity_id('certification_rule' ,$certification_rule);
  if (!empty($certification_rule_id) && certification_rule_access('edit', $certification_rule)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('certification_rule_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'certification_rule_form_validate';

  return $form;
}


/**
 * Certification rule submit validation handler.
 */
function certification_rule_form_validate($form, &$form_state) {
  //TODO: do some actual validation here

}

/**
 * Certification rule submit handler.
 */
function certification_rule_form_submit($form, &$form_state) {
  $certification_rule = $form_state['certification_rule'];
  entity_form_submit_build_entity('certification_rule', $certification_rule, $form, $form_state);
  certification_rule_save($certification_rule);
  $certification_rule_uri = entity_uri('certification_rule', $certification_rule);
  $form_state['redirect'] = $certification_rule_uri['path'];
  drupal_set_message(t('Certification Rule %title saved.', array('%title' => entity_label('certification_rule', $certification_rule))));
}


/**
 * Certification rule submit delete handler.
 */
function certification_rule_form_submit_delete($form, &$form_state) {
  $certification_rule = $form_state['certification_rule'];
  $certification_rule_uri = entity_uri('certification_rule', $certification_rule);
  $form_state['redirect'] = $certification_rule_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function certification_rule_delete_form($form, &$form_state, $certification_rule) {
  $form_state['certification_rule'] = $certification_rule;
  $certification_rule_uri = entity_uri('certification_rule', $certification_rule);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => entity_label('certification_rule', $certification_rule))),
    $certification_rule_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function certification_rule_delete_form_submit($form, &$form_state) {
  $certification_rule = $form_state['certification_rule'];
  certification_rule_delete($certification_rule);
  drupal_set_message(t('Certification rule %title deleted.', array('%title' => entity_label('certification_rule', $certification_rule))));
  $form_state['redirect'] = '<front>';
}

/**
 * Sync confirmation form.
 */
function certification_rule_sync_form($form, &$form_state, $certification_rule) {
  $form_state['certification_rule'] = $certification_rule;
  $certification_rule_uri = entity_uri('certification_rule', $certification_rule);
  return confirm_form($form,
    t('Sync %title?', array('%title' => entity_label('certification_rule', $certification_rule))),
    $certification_rule_uri['path'],
    t('This action cannot be undone.'),
    t('Sync'),
    t('Cancel')
  );
}

/**
 *
 * */
function certification_rule_sync_form_submit($form, &$form_state) {
  $certification_rule = $form_state['certification_rule'];
  $certification_rule_uri = entity_uri('certification_rule', $certification_rule);
  certification_rule_sync($certification_rule);
  $form_state['redirect'] = "{$certification_rule_uri['path']}/$certification_rule->cert_rule_id"; // TODO: should we go to the front page?
}

