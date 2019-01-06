<?php

/**
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2018
 *
 * Generated from xml/schema/CRM/Campaign/Survey.xml
 * DO NOT EDIT.  Generated by CRM_Core_CodeGen
 * (GenCodeChecksum:55aa9d31d08f8b5353d42dde1278b7f2)
 */

/**
 * Database access object for the Survey entity.
 */
class CRM_Campaign_DAO_Survey extends CRM_Core_DAO {

  /**
   * Static instance to hold the table name.
   *
   * @var string
   */
  static $_tableName = 'civicrm_survey';

  /**
   * Should CiviCRM log any modifications to this table in the civicrm_log table.
   *
   * @var bool
   */
  static $_log = FALSE;

  /**
   * Survey id.
   *
   * @var int unsigned
   */
  public $id;

  /**
   * Title of the Survey.
   *
   * @var string
   */
  public $title;

  /**
   * Foreign key to the Campaign.
   *
   * @var int unsigned
   */
  public $campaign_id;

  /**
   * Implicit FK to civicrm_option_value where option_group = activity_type
   *
   * @var int unsigned
   */
  public $activity_type_id;

  /**
   * Recontact intervals for each status.
   *
   * @var text
   */
  public $recontact_interval;

  /**
   * Script instructions for volunteers to use for the survey.
   *
   * @var text
   */
  public $instructions;

  /**
   * Number of days for recurrence of release.
   *
   * @var int unsigned
   */
  public $release_frequency;

  /**
   * Maximum number of contacts to allow for survey.
   *
   * @var int unsigned
   */
  public $max_number_of_contacts;

  /**
   * Default number of contacts to allow for survey.
   *
   * @var int unsigned
   */
  public $default_number_of_contacts;

  /**
   * Is this survey enabled or disabled/cancelled?
   *
   * @var boolean
   */
  public $is_active;

  /**
   * Is this default survey?
   *
   * @var boolean
   */
  public $is_default;

  /**
   * FK to civicrm_contact, who created this Survey.
   *
   * @var int unsigned
   */
  public $created_id;

  /**
   * Date and time that Survey was created.
   *
   * @var datetime
   */
  public $created_date;

  /**
   * FK to civicrm_contact, who recently edited this Survey.
   *
   * @var int unsigned
   */
  public $last_modified_id;

  /**
   * Date and time that Survey was edited last time.
   *
   * @var datetime
   */
  public $last_modified_date;

  /**
   * Used to store option group id.
   *
   * @var int unsigned
   */
  public $result_id;

  /**
   * Bypass the email verification.
   *
   * @var boolean
   */
  public $bypass_confirm;

  /**
   * Title for Thank-you page (header title tag, and display at the top of the page).
   *
   * @var string
   */
  public $thankyou_title;

  /**
   * text and html allowed. displayed above result on success page
   *
   * @var text
   */
  public $thankyou_text;

  /**
   * Can people share the petition through social media?
   *
   * @var boolean
   */
  public $is_share;

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->__table = 'civicrm_survey';
    parent::__construct();
  }

  /**
   * Returns foreign keys and entity references.
   *
   * @return array
   *   [CRM_Core_Reference_Interface]
   */
  public static function getReferenceColumns() {
    if (!isset(Civi::$statics[__CLASS__]['links'])) {
      Civi::$statics[__CLASS__]['links'] = static ::createReferenceColumns(__CLASS__);
      Civi::$statics[__CLASS__]['links'][] = new CRM_Core_Reference_Basic(self::getTableName(), 'campaign_id', 'civicrm_campaign', 'id');
      Civi::$statics[__CLASS__]['links'][] = new CRM_Core_Reference_Basic(self::getTableName(), 'created_id', 'civicrm_contact', 'id');
      Civi::$statics[__CLASS__]['links'][] = new CRM_Core_Reference_Basic(self::getTableName(), 'last_modified_id', 'civicrm_contact', 'id');
      CRM_Core_DAO_AllCoreTables::invoke(__CLASS__, 'links_callback', Civi::$statics[__CLASS__]['links']);
    }
    return Civi::$statics[__CLASS__]['links'];
  }

  /**
   * Returns all the column names of this table
   *
   * @return array
   */
  public static function &fields() {
    if (!isset(Civi::$statics[__CLASS__]['fields'])) {
      Civi::$statics[__CLASS__]['fields'] = [
        'id' => [
          'name' => 'id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey ID'),
          'description' => ts('Survey id.'),
          'required' => TRUE,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'title' => [
          'name' => 'title',
          'type' => CRM_Utils_Type::T_STRING,
          'title' => ts('Survey Title'),
          'description' => ts('Title of the Survey.'),
          'required' => TRUE,
          'maxlength' => 255,
          'size' => CRM_Utils_Type::HUGE,
          'import' => TRUE,
          'where' => 'civicrm_survey.title',
          'headerPattern' => '',
          'dataPattern' => '',
          'export' => TRUE,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 1,
        ],
        'campaign_id' => [
          'name' => 'campaign_id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey Campaign ID'),
          'description' => ts('Foreign key to the Campaign.'),
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
          'FKClassName' => 'CRM_Campaign_DAO_Campaign',
          'pseudoconstant' => [
            'table' => 'civicrm_campaign',
            'keyColumn' => 'id',
            'labelColumn' => 'title',
          ]
        ],
        'activity_type_id' => [
          'name' => 'activity_type_id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Activity Type'),
          'description' => ts('Implicit FK to civicrm_option_value where option_group = activity_type'),
          'import' => TRUE,
          'where' => 'civicrm_survey.activity_type_id',
          'headerPattern' => '',
          'dataPattern' => '',
          'export' => TRUE,
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
          'html' => [
            'type' => 'Select',
          ],
          'pseudoconstant' => [
            'optionGroupName' => 'activity_type',
            'optionEditPath' => 'civicrm/admin/options/activity_type',
          ]
        ],
        'recontact_interval' => [
          'name' => 'recontact_interval',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Follow up Interval'),
          'description' => ts('Recontact intervals for each status.'),
          'rows' => 20,
          'cols' => 80,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
          'html' => [
            'type' => 'TextArea',
          ],
        ],
        'instructions' => [
          'name' => 'instructions',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Instructions'),
          'description' => ts('Script instructions for volunteers to use for the survey.'),
          'rows' => 20,
          'cols' => 80,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 1,
          'html' => [
            'type' => 'TextArea',
          ],
        ],
        'release_frequency' => [
          'name' => 'release_frequency',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey Hold Duration'),
          'description' => ts('Number of days for recurrence of release.'),
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'max_number_of_contacts' => [
          'name' => 'max_number_of_contacts',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Maximum number of contacts'),
          'description' => ts('Maximum number of contacts to allow for survey.'),
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'default_number_of_contacts' => [
          'name' => 'default_number_of_contacts',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Default number of contacts'),
          'description' => ts('Default number of contacts to allow for survey.'),
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'is_active' => [
          'name' => 'is_active',
          'type' => CRM_Utils_Type::T_BOOLEAN,
          'title' => ts('Survey Is Active'),
          'description' => ts('Is this survey enabled or disabled/cancelled?'),
          'default' => '1',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'is_default' => [
          'name' => 'is_default',
          'type' => CRM_Utils_Type::T_BOOLEAN,
          'title' => ts('Is Default Survey'),
          'description' => ts('Is this default survey?'),
          'default' => '0',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'created_id' => [
          'name' => 'created_id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey Created By'),
          'description' => ts('FK to civicrm_contact, who created this Survey.'),
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
          'FKClassName' => 'CRM_Contact_DAO_Contact',
        ],
        'created_date' => [
          'name' => 'created_date',
          'type' => CRM_Utils_Type::T_DATE + CRM_Utils_Type::T_TIME,
          'title' => ts('Campaign Created Date'),
          'description' => ts('Date and time that Survey was created.'),
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'last_modified_id' => [
          'name' => 'last_modified_id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey Modified'),
          'description' => ts('FK to civicrm_contact, who recently edited this Survey.'),
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
          'FKClassName' => 'CRM_Contact_DAO_Contact',
        ],
        'last_modified_date' => [
          'name' => 'last_modified_date',
          'type' => CRM_Utils_Type::T_DATE + CRM_Utils_Type::T_TIME,
          'title' => ts('Survey Modified On'),
          'description' => ts('Date and time that Survey was edited last time.'),
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'result_id' => [
          'name' => 'result_id',
          'type' => CRM_Utils_Type::T_INT,
          'title' => ts('Survey Result'),
          'description' => ts('Used to store option group id.'),
          'default' => 'NULL',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'bypass_confirm' => [
          'name' => 'bypass_confirm',
          'type' => CRM_Utils_Type::T_BOOLEAN,
          'title' => ts('No Email Verification'),
          'description' => ts('Bypass the email verification.'),
          'default' => '0',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
        'thankyou_title' => [
          'name' => 'thankyou_title',
          'type' => CRM_Utils_Type::T_STRING,
          'title' => ts('Thank-you Title'),
          'description' => ts('Title for Thank-you page (header title tag, and display at the top of the page).'),
          'maxlength' => 255,
          'size' => CRM_Utils_Type::HUGE,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 1,
        ],
        'thankyou_text' => [
          'name' => 'thankyou_text',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Thank-you Text'),
          'description' => ts('text and html allowed. displayed above result on success page'),
          'rows' => 8,
          'cols' => 60,
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 1,
          'html' => [
            'type' => 'TextArea',
          ],
        ],
        'is_share' => [
          'name' => 'is_share',
          'type' => CRM_Utils_Type::T_BOOLEAN,
          'title' => ts('Is shared through social media'),
          'description' => ts('Can people share the petition through social media?'),
          'default' => '1',
          'table_name' => 'civicrm_survey',
          'entity' => 'Survey',
          'bao' => 'CRM_Campaign_BAO_Survey',
          'localizable' => 0,
        ],
      ];
      CRM_Core_DAO_AllCoreTables::invoke(__CLASS__, 'fields_callback', Civi::$statics[__CLASS__]['fields']);
    }
    return Civi::$statics[__CLASS__]['fields'];
  }

  /**
   * Return a mapping from field-name to the corresponding key (as used in fields()).
   *
   * @return array
   *   Array(string $name => string $uniqueName).
   */
  public static function &fieldKeys() {
    if (!isset(Civi::$statics[__CLASS__]['fieldKeys'])) {
      Civi::$statics[__CLASS__]['fieldKeys'] = array_flip(CRM_Utils_Array::collect('name', self::fields()));
    }
    return Civi::$statics[__CLASS__]['fieldKeys'];
  }

  /**
   * Returns the names of this table
   *
   * @return string
   */
  public static function getTableName() {
    return CRM_Core_DAO::getLocaleTableName(self::$_tableName);
  }

  /**
   * Returns if this table needs to be logged
   *
   * @return bool
   */
  public function getLog() {
    return self::$_log;
  }

  /**
   * Returns the list of fields that can be imported
   *
   * @param bool $prefix
   *
   * @return array
   */
  public static function &import($prefix = FALSE) {
    $r = CRM_Core_DAO_AllCoreTables::getImports(__CLASS__, 'survey', $prefix, []);
    return $r;
  }

  /**
   * Returns the list of fields that can be exported
   *
   * @param bool $prefix
   *
   * @return array
   */
  public static function &export($prefix = FALSE) {
    $r = CRM_Core_DAO_AllCoreTables::getExports(__CLASS__, 'survey', $prefix, []);
    return $r;
  }

  /**
   * Returns the list of indices
   *
   * @param bool $localize
   *
   * @return array
   */
  public static function indices($localize = TRUE) {
    $indices = [
      'UI_activity_type_id' => [
        'name' => 'UI_activity_type_id',
        'field' => [
          0 => 'activity_type_id',
        ],
        'localizable' => FALSE,
        'sig' => 'civicrm_survey::0::activity_type_id',
      ],
    ];
    return ($localize && !empty($indices)) ? CRM_Core_DAO_AllCoreTables::multilingualize(__CLASS__, $indices) : $indices;
  }

}
