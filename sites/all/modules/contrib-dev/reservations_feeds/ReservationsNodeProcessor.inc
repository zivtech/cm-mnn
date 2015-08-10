<?php

/**
 * @file
 * Class definition of FeedsNodeProcessor.
 */

/**
 * Creates nodes from feed items.
 */
class ReservationsNodeProcessor extends FeedsNodeProcessor {

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['content_type']['#description'] .= t('  Content type can be overridden by mapping.');
    return $form;
  }

  /**
   * Creates a new node in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $node = parent::newEntity($source);
    $node->reservations_sub_type = RESERVATIONS_SUB_TYPE_ITEM; 
    return $node;
  }
  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();

    $targets += array(
      'type' => array(
        'name' => t('Content type'),
        'description' => t('The content type of the node.'),
      ),
      'reservations_default_availability' => array(
        'name' => t('Reservations default availability'),
        'description' => 'Default availability of the item .',
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
    );

    return $targets;
  }

}

