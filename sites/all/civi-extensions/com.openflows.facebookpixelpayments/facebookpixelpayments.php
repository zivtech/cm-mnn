<?php

require_once 'facebookpixelpayments.civix.php';
use CRM_Facebookpixelpayments_ExtensionUtil as E;
/**
 * Implementation of hook_civicrm_buildForm
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_buildForm
 */
function facebookpixelpayments_civicrm_buildForm($formName, &$form) {
#add a script to the page if it is an event registration page
  if($formName == 'CRM_Event_Form_Registration_Register') { 
//    CRM_Core_Resources::singleton()->addScriptFile('com.openflows.facebookpixelpayments', 'facebook_pixel_reg_start.js');
  }
#add a script to the page if it the payment/confirmation page
  if($formName == 'CRM_Event_Form_Registration_Confirm') { 
    CRM_Core_Resources::singleton()->addScriptFile('com.openflows.facebookpixelpayments', 'facebook_pixel_reg_checkout.js');
  }  
  
#add a script to the page if it is an event registration complete/thank you page
  if($formName == 'CRM_Event_Form_Registration_ThankYou') {
    CRM_Core_Resources::singleton()->addScriptFile('com.openflows.facebookpixelpayments', 'facebook_pixel_reg_complete.js');
  }
  

  
}
/**
 * Implements hook_civicrm_config().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_config
 */
function facebookpixelpayments_civicrm_config(&$config) {
  _facebookpixelpayments_civix_civicrm_config($config);
}

/**
 * Implements hook_civicrm_xmlMenu().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_xmlMenu
 */
function facebookpixelpayments_civicrm_xmlMenu(&$files) {
  _facebookpixelpayments_civix_civicrm_xmlMenu($files);
}

/**
 * Implements hook_civicrm_install().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_install
 */
function facebookpixelpayments_civicrm_install() {
  _facebookpixelpayments_civix_civicrm_install();
}

/**
 * Implements hook_civicrm_postInstall().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_postInstall
 */
function facebookpixelpayments_civicrm_postInstall() {
  _facebookpixelpayments_civix_civicrm_postInstall();
}

/**
 * Implements hook_civicrm_uninstall().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_uninstall
 */
function facebookpixelpayments_civicrm_uninstall() {
  _facebookpixelpayments_civix_civicrm_uninstall();
}

/**
 * Implements hook_civicrm_enable().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_enable
 */
function facebookpixelpayments_civicrm_enable() {
  _facebookpixelpayments_civix_civicrm_enable();
}

/**
 * Implements hook_civicrm_disable().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_disable
 */
function facebookpixelpayments_civicrm_disable() {
  _facebookpixelpayments_civix_civicrm_disable();
}

/**
 * Implements hook_civicrm_upgrade().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_upgrade
 */
function facebookpixelpayments_civicrm_upgrade($op, CRM_Queue_Queue $queue = NULL) {
  return _facebookpixelpayments_civix_civicrm_upgrade($op, $queue);
}

/**
 * Implements hook_civicrm_managed().
 *
 * Generate a list of entities to create/deactivate/delete when this module
 * is installed, disabled, uninstalled.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_managed
 */
function facebookpixelpayments_civicrm_managed(&$entities) {
  _facebookpixelpayments_civix_civicrm_managed($entities);
}

/**
 * Implements hook_civicrm_caseTypes().
 *
 * Generate a list of case-types.
 *
 * Note: This hook only runs in CiviCRM 4.4+.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_caseTypes
 */
function facebookpixelpayments_civicrm_caseTypes(&$caseTypes) {
  _facebookpixelpayments_civix_civicrm_caseTypes($caseTypes);
}

/**
 * Implements hook_civicrm_angularModules().
 *
 * Generate a list of Angular modules.
 *
 * Note: This hook only runs in CiviCRM 4.5+. It may
 * use features only available in v4.6+.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_angularModules
 */
function facebookpixelpayments_civicrm_angularModules(&$angularModules) {
  _facebookpixelpayments_civix_civicrm_angularModules($angularModules);
}

/**
 * Implements hook_civicrm_alterSettingsFolders().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_alterSettingsFolders
 */
function facebookpixelpayments_civicrm_alterSettingsFolders(&$metaDataFolders = NULL) {
  _facebookpixelpayments_civix_civicrm_alterSettingsFolders($metaDataFolders);
}

/**
 * Implements hook_civicrm_entityTypes().
 *
 * Declare entity types provided by this module.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_entityTypes
 */
function facebookpixelpayments_civicrm_entityTypes(&$entityTypes) {
  _facebookpixelpayments_civix_civicrm_entityTypes($entityTypes);
}

// --- Functions below this ship commented out. Uncomment as required. ---

/**
 * Implements hook_civicrm_preProcess().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_preProcess
 *
function facebookpixelpayments_civicrm_preProcess($formName, &$form) {

} // */

/**
 * Implements hook_civicrm_navigationMenu().
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_navigationMenu
 *
function facebookpixelpayments_civicrm_navigationMenu(&$menu) {
  _facebookpixelpayments_civix_insert_navigation_menu($menu, 'Mailings', array(
    'label' => E::ts('New subliminal message'),
    'name' => 'mailing_subliminal_message',
    'url' => 'civicrm/mailing/subliminal',
    'permission' => 'access CiviMail',
    'operator' => 'OR',
    'separator' => 0,
  ));
  _facebookpixelpayments_civix_navigationMenu($menu);
} // */
