<?php
/**
 * Extends the EntityAPIController class
 *
 */
class CertificationRuleController extends EntityAPIController {

  /**
   * Create a default new entity
   *
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'cert_rule_id' => '',
      'cert_rule_name' => '',
      'cert_rule_event_type' => '',
      'cert_rule_status' => '',
      'cert_rule_created' => 'REQUEST_TIME',
      'cert_rule_group' => '',
    );
    return parent::create($values);
  }

  /**
   * Build an existing entity
   * provide metadata wrapper
   *
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('certification_rule', $entity);
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * CertificationType class.
 */
class CertificationRule extends Entity {
  /**
   * provides the label for the entity
   *
   */
  protected function defaultLabel() {
    return $this->cert_rule_name;
  }

  /**
   * provides the default path for the certification rule
   *
   */
  protected function defaultUri() {
    return array('path' => 'certification-rule/' . $this->identifier());
  }
}
