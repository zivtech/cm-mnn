<?php

/**
 * @file
 * Class definition of FeedsNodeProcessor.
 */

/**
 * Creates nodes from feed items.
 */
class ReservationsTypeProcessor extends FeedsProcessor {

  /**
   * Entity type this processor operates on.
   */
  public function entityType() {
    return 'reservations_type';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label'] = t('Reservations type');
    $info['label plural'] = t('Reservations types');
    return $info;
  }
  /**
   * Create a new entity.
   *
   * @param $source
   *   The feeds source that spawns this entity.
   *
   * @return
   *   A new entity object.
   */
  protected function newEntity(FeedsSource $source) {
    return new stdClass();
  }

  /**
   * Load an existing entity.
   *
   * @param $source
   *   The feeds source that spawns this entity.
   * @param $entity_id
   *   The unique id of the entity that should be loaded.
   *
   * @return
   *   A new entity object.
   */
  protected function entityLoad(FeedsSource $source, $entity_id){
  }

  /**
   * Save an entity.
   *
   * @param $entity
   *   Entity to b saved.
   */
  protected function entitySave($entity) {
    $form_state['values'] = (array) $entity;
    $form_state['values']['orig_type'] = $form_state['values']['type'];
    $form_state['values']['base'] = 'node_content';
    $form_state['values']['op'] = t('Save content type');
    module_load_include('inc', 'node', 'content_types');
    drupal_form_submit('node_type_form', $form_state);
  }

  /**
   * Delete a series of entities.
   *
   * @param $entity_ids
   *   Array of unique identity ids to be deleted.
   */
  protected function entityDeleteMultiple($entity_ids){
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {

    $targets = array(
      'type' => array(
        'name' => t('Content type'),
        'description' => t('The content type.'),
      ),
      'name' => array(
        'name' => t('Content type name'),
        'description' => t('The content type name.'),
      ),
      'description' => array(
        'name' => t('Content type description'),
        'description' => t('The content type description.'),
      ),
      'reservations_type_setting' => array(
        'name' => t('Reservations type setting'),
        'description' => "The Reservations type for the selected content type.",
      ),
      'reservations_max_hours_per_reservation' => array(
        'name'        => t('Reservations max hours per reservation'),
        'description' => "The maximum hours that the resource can be reserved for.",
      ),
      'reservations_allow_overnight' => array(
        'name'        => t('Reservations allow overnight reservation'),
        'description' => "TRUE if the resource can be kept overnight.",
      ),
      'reservations_allow_weekends' => array(
        'name'        => t('Reservations allow weekend reservation'),
        'description' => "TRUE if the resource can be kept over weekends.",
      ),
      'reservations_late_fee_per_hour' => array(
        'name' => t('Reservations late fee per hour'),
        'description' => "Per hour fee if the resource is returned late.",
      ),
      'reservations_rate_per_hour' => array(
        'name' => t('Reservations rate per hour'),
        'description' => "Per hour fee for use of the resource.",
      ),
      'reservations_fee_free_hours' => array(
        'name' => t('Reservations fee free hours'),
        'description' => "Number of hours the item can be used before fees are charged.",
      ),
      'reservations_active_status' => array(
        'name'        => t('Reservations active status'),
        'description' => "The status of the resource.",
      ),
      'reservations_spare_items' => array(
        'name'        => t('Reservations spare items'),
        'description' => "Number of spare items to leave unreserved for the resource.",
      ),
      'reservations_min_cancel_hours' => array(
        'name' => t('Reservations min cancel hours'),
        'description' => "Minimum number of hours a user can cancel a reservation for the item.",
      ),
      'reservations_autocheckout' => array(
        'name' => t('Reservations autocheckout'),
        'description' => "TRUE if the resource is automatically checked out when Reservation starts.",
      ),
      'reservations_autocheckin' => array(
        'name' => t('Reservations autocheckin'),
        'description' => "TRUE if the resource is automatically checked in when Reservation starts.",
      ),
      'reservations_selfcheckout' => array(
        'name' => t('Reservations selfcheckout'),
        'description' => "TRUE if the resource is managed by selfcheckout.",
      ),
      'reservations_grouping' => array(
        'name' => t('Reservations grouping'),
        'description' => "tid of Reservations Equiment Grouping vocabulary",
      ),
      'reservations_auto_assign_bucket_item' => array(
        'name' => t('Reservations auto assign'),
        'description' => "TRUE if the bucket item is to be assigned automatically.",
      ),
    );

    return $targets;
  }

}


