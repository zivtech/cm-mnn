<?php
/**
 * Extends the EntityAPIController class
 *
 */
class CertificationController extends EntityAPIController {

  /**
   * Create a default new entity
   *
   */
  public function create(array $values = array()) {

    $values += array(
      'cert_id' => '',
      'cert_created' => REQUEST_TIME,
      'cert_rule_name' => '',
    );
    return parent::create($values);
  }



  /**
   * Build an existing entity
   * provide metadata wrapper
   *
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $wrapper = entity_metadata_wrapper('certification', $entity);
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * CertificationType class.
 */
class Certification extends Entity {
  /**
   * provides the label for the entity
   *
   */
  protected function defaultLabel() {
    return 'Certification #' . $this->cert_id;
  }

  /**
   * provides the default path for the certification type
   *
   */
  protected function defaultUri() {
    return array('path' => 'certification/' . $this->identifier());
  }
}
