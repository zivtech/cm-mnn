<?php

/**
 * 'Certification' entity view callback.
 * View of a single certification entity
 */
function certification_view($certification) {
  drupal_set_title(entity_label('certification', $certification, $view_mode = "full"));

  // Our entity type, for convenience.
  $entity_type = 'certification';
  // Start setting up the content.
  $certification->content = array(
    '#view_mode' => $view_mode,
  );

  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($certification->cert_id => $certification), $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($certification->cert_id => $certification));
  // Now field_attach_view() generates the content for the fields.
  $certification->content += field_attach_view($entity_type, $certification, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $certification->content['cert_event'] = array(
    '#type' => 'item',
    '#title' => t('Event title'),
    '#markup' => civicrm_certify_event_display($certification->cert_event),
  );
  $certification->content['cert_user'] = array(
    '#type' => 'item',
    '#title' => t('Certified user'),
    '#markup' => civicrm_certify_contact_link($certification->cert_user),
  );
  $certification->content['cert_created'] = array(
    '#type' => 'item',
    '#title' => t('Certification last saved'),
    '#markup' => format_date($certification->cert_created, 'custom', 'Y-m-d H:i:s O'),
  );
  $certification->content['cert_date'] = array(
    '#type' => 'item',
    '#title' => t('Certified On'),
    '#markup' => format_date($certification->cert_date, 'custom', 'Y-m-d H:i:s O'),
  );
  $certification->content['cert_author'] = array(
    '#type' => 'item',
    '#title' => t('Authorized By'),
    '#markup' => civicrm_certify_contact_link($certification->cert_author),
  );
  $certification->content['cert_rule_name'] = array(
    '#type' => 'item',
    '#title' => t('Certification Name'),
    '#markup' => civicrm_certify_cert_name($certification),
  );

  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language ;
  $langcode = $language->language ;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $certification, $view_mode, $langcode);

  // Now invoke hook_entity_view_alter().
   drupal_alter(array('certification_view', 'entity_view'),
    $certification->content, $entity_type);

  // And finally return the content.
  return $certification->content;
}

/**
 * Certification content page callback
 */

function certification_content() {
  // we are going to output the results in a table with a nice header
  $header = array(
    array('data' => t('Certification Rule')),
    array('data' => t('Workshop')),
    array('data' => t('Certified On')),
    array('data' => t('Certified User')),
    array('data' => t('Authorized By')),
    array('data' => t('Operations')),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'certification')
    ->pager(25)
    ->entityOrderBy('entity_id', 'DESC')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();

  $rows = array();
  if (isset($result['certification'])) {
    $cert_ids = array_keys($result['certification']);
    $certs = entity_load('certification', $cert_ids);

    foreach ( $certs as $entity ) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          //'title' => t("<a href='/certification-rule/{$entity->cert_rule_id}'>$entity->cert_rule_name</a>"),
          'Certification Name' => civicrm_certify_cert_name($entity),
          'Workshop Name' => civicrm_certify_event_display($entity->cert_event),
          'Certified On' => format_date($entity->cert_created, 'custom', 'Y-m-d H:i:s O'),
          'Certified User' => civicrm_certify_contact_link($entity->cert_user),
          'Authorized By' => civicrm_certify_contact_link($entity->cert_author),
          'Operations' => "<ul class='links inline'>
                            <li class='edit first'><a href='/certification/{$entity->cert_id}/view'>view</a></li>
                            <li class='edit last'><a href='/certification/{$entity->cert_id}/delete'>delete</a></li>
                          </ul>",
        ),
      );
    }
  }
  // Create a render array ($build) which will be themed as a table with a
  // pager.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no certifications found'),
  );

  // attach the pager theme
  $build['pager_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Certification_tab callback
 * return data suitable for a civicrm tab
 */
function civicrm_certify_certification_tab($cid) {
  // we are going to output the results in a table with a nice header
  $header = array(
    array('data' => t('Certification Name')),
    array('data' => t('Workshop')),
    array('data' => t('Date certified')),
    array('data' => t('Authorized By')),
    array('data' => t('Operations')),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'certification')
    ->propertyCondition('cert_user', $cid)
    ->pager(25)
    ->entityOrderBy('entity_id', 'DESC')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();

  $rows = array();
  if (isset($result['certification'])) {
    $cert_ids = array_keys($result['certification']);
    $certs = entity_load('certification', $cert_ids);
    $display = array(
      'label' => 'hidden',
      'settings' => array(
        'link' => 1,
      ),
    );

    foreach ( $certs as $entity ) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          //'title' => t("<a href='/certification-rule/{$entity->cert_rule_id}'>$entity->cert_rule_name</a>"),
          'Certification Name' => civicrm_certify_cert_name($entity),
          'Workshop Name' => civicrm_certify_event_display($entity->cert_event),
          'Certified On' => format_date($entity->cert_created, 'custom', 'Y-m-d H:i:s O'),
          'Authorized By' => civicrm_certify_contact_link($entity->cert_author),
          'operations' => "<ul class='links inline'>
                            <li class='edit first'><a href='/certification/{$entity->cert_id}/view'>view</a></li>
                          </ul>",
        ),
      );
    }
  }
  // Create a render array ($build) which will be themed as a table with a
  // pager.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no certifications found'),
  );

  // attach the pager theme
  $build['pager_pager'] = array('#theme' => 'pager');

  print render($build);
  return;
}

/**
 * Certification_profile_tab callback
 * Displays the certifications for a drupal contact
 *
 * @params
 *   uid = the drupal user id
 */
function civicrm_certify_certification_profile_tab($uid) {
  // first things first, we need to get the full contact
  $contact = _civicrm_certify_civicrm_ufmatch($uid);

  if (!$cid = $contact['contact_id']) { return "There was an error. Someone should do something.";}

  // we are going to output the results in a table with a nice header
  $header = array(
    array('data' => t('Certification Name')),
    array('data' => t('Workshop')),
    array('data' => t('Date certified')),
    array('data' => t('Operations')),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'certification')
    ->propertyCondition('cert_user', $cid)
    ->pager(25)
    ->entityOrderBy('entity_id', 'DESC')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();

  $rows = array();
  if (isset($result['certification'])) {
    $cert_ids = array_keys($result['certification']);
    $certs = entity_load('certification', $cert_ids);
    $display = array(
      'label' => 'hidden',
      'settings' => array(
        'link' => 1,
      ),
    );

    foreach ( $certs as $entity ) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          //'title' => t("<a href='/certification-rule/{$entity->cert_rule_id}'>$entity->cert_rule_name</a>"),
          'Certification Name' => civicrm_certify_cert_name($entity, "text"),
          'Workshop Name' => civicrm_certify_event_display($entity->cert_event),
          'Certified On' => format_date($entity->cert_created, 'custom', 'Y-m-d H:i:s O'),
          'operations' => "<ul class='links inline'>
                            <li class='edit first'><a href='/certification/{$entity->cert_id}/view'>view</a></li>
                          </ul>",
        ),
      );
    }
  }
  // Create a render array ($build) which will be themed as a table with a
  // pager.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no certifications found'),
  );

  // attach the pager theme
  $build['pager_pager'] = array('#theme' => 'pager');

  return render($build);
}

/**
 * Certification Type edit (new) form.
 */
function certification_form($form, &$form_state, $certification) {
  $form_state['certification'] = $certification;

  $form['cert_id'] = array(
    '#type' => 'hidden',
    '#title' => t('cert_id'),
    '#default_value' => $certification->cert_id,
  );

  field_attach_form('certification', $certification, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  // Show Delete button if we edit certification.
  $certification_id = entity_id('certification' ,$certification);
  if (!empty($certification_id) && certification_access('edit', $certification)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('certification_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'certification_form_validate';

  return $form;
}



/**
 * Certification submit validation handler.
 */
function certification_form_validate($form, &$form_state) {

}

/**
 * Certification submit handler.
 */
function certification_form_submit($form, &$form_state) {
  $certification = $form_state['certification'];
  entity_form_submit_build_entity('certification', $certification, $form, $form_state);
  certification_save($certification);
  $certification_uri = entity_uri('certification', $certification);
  $form_state['redirect'] = $certification_uri['path'];
  drupal_set_message(t('Certification %title saved.', array('%title' => entity_label('certification', $certification))));
}


/**
 * Certification submit delete handler.
 */
function certification_form_submit_delete($form, &$form_state) {
  $certification = $form_state['certification'];
  $certification_uri = entity_uri('certification', $certification);
  $form_state['redirect'] = $certification_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function certification_delete_form($form, &$form_state, $certification) {
  $form_state['certification'] = $certification;
  $certification_uri = entity_uri('certification', $certification);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => entity_label('certification', $certification))),
    $certification_uri['path'],
    t('This action will remove the certification for %user.', array('%user' => civicrm_certify_contact_link($certification->cert_user, "text"))),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function certification_delete_form_submit($form, &$form_state) {
  $certification = $form_state['certification'];
  certification_delete($certification);
  drupal_set_message(t('%title deleted.', array('%title' => entity_label('certification', $certification))));
  $form_state['redirect'] = '<front>';
}
