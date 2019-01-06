<?php

/**
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2018
 *
 * Generated from xml/schema/CRM/Cxn/Cxn.xml
 * DO NOT EDIT.  Generated by CRM_Core_CodeGen
 * (GenCodeChecksum:90ebdb9687fcc28fb71ec5ef15dafc1b)
 */

/**
 * Database access object for the Cxn entity.
 */
class CRM_Cxn_DAO_Cxn extends CRM_Core_DAO {

  /**
   * Static instance to hold the table name.
   *
   * @var string
   */
  static $_tableName = 'civicrm_cxn';

  /**
   * Should CiviCRM log any modifications to this table in the civicrm_log table.
   *
   * @var bool
   */
  static $_log = FALSE;

  /**
   * Connection ID
   *
   * @var int unsigned
   */
  public $id;

  /**
   * Application GUID
   *
   * @var string
   */
  public $app_guid;

  /**
   * Application Metadata (JSON)
   *
   * @var text
   */
  public $app_meta;

  /**
   * Connection GUID
   *
   * @var string
   */
  public $cxn_guid;

  /**
   * Shared secret
   *
   * @var text
   */
  public $secret;

  /**
   * Permissions approved for the service (JSON)
   *
   * @var text
   */
  public $perm;

  /**
   * Options for the service (JSON)
   *
   * @var text
   */
  public $options;

  /**
   * Is connection currently enabled?
   *
   * @var boolean
   */
  public $is_active;

  /**
   * When was the connection was created.
   *
   * @var timestamp
   */
  public $created_date;

  /**
   * When the connection was created or modified.
   *
   * @var timestamp
   */
  public $modified_date;

  /**
   * The last time the application metadata was fetched.
   *
   * @var timestamp
   */
  public $fetched_date;

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->__table = 'civicrm_cxn';
    parent::__construct();
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
          'title' => ts('Connection ID'),
          'description' => ts('Connection ID'),
          'required' => TRUE,
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'app_guid' => [
          'name' => 'app_guid',
          'type' => CRM_Utils_Type::T_STRING,
          'title' => ts('Application GUID'),
          'description' => ts('Application GUID'),
          'maxlength' => 128,
          'size' => CRM_Utils_Type::HUGE,
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'app_meta' => [
          'name' => 'app_meta',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Application Metadata (JSON)'),
          'description' => ts('Application Metadata (JSON)'),
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'cxn_guid' => [
          'name' => 'cxn_guid',
          'type' => CRM_Utils_Type::T_STRING,
          'title' => ts('Connection GUID'),
          'description' => ts('Connection GUID'),
          'maxlength' => 128,
          'size' => CRM_Utils_Type::HUGE,
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'secret' => [
          'name' => 'secret',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Secret'),
          'description' => ts('Shared secret'),
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'perm' => [
          'name' => 'perm',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Perm'),
          'description' => ts('Permissions approved for the service (JSON)'),
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'options' => [
          'name' => 'options',
          'type' => CRM_Utils_Type::T_TEXT,
          'title' => ts('Options'),
          'description' => ts('Options for the service (JSON)'),
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
          'serialize' => self::SERIALIZE_JSON,
        ],
        'is_active' => [
          'name' => 'is_active',
          'type' => CRM_Utils_Type::T_BOOLEAN,
          'title' => ts('Is Active'),
          'description' => ts('Is connection currently enabled?'),
          'default' => '1',
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'created_date' => [
          'name' => 'created_date',
          'type' => CRM_Utils_Type::T_TIMESTAMP,
          'title' => ts('Created Date'),
          'description' => ts('When was the connection was created.'),
          'required' => FALSE,
          'default' => 'NULL',
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'modified_date' => [
          'name' => 'modified_date',
          'type' => CRM_Utils_Type::T_TIMESTAMP,
          'title' => ts('Modified Date'),
          'description' => ts('When the connection was created or modified.'),
          'required' => FALSE,
          'default' => 'CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP',
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
          'localizable' => 0,
        ],
        'fetched_date' => [
          'name' => 'fetched_date',
          'type' => CRM_Utils_Type::T_TIMESTAMP,
          'title' => ts('Fetched Date'),
          'description' => ts('The last time the application metadata was fetched.'),
          'required' => FALSE,
          'default' => 'NULL',
          'table_name' => 'civicrm_cxn',
          'entity' => 'Cxn',
          'bao' => 'CRM_Cxn_BAO_Cxn',
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
    return self::$_tableName;
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
    $r = CRM_Core_DAO_AllCoreTables::getImports(__CLASS__, 'cxn', $prefix, []);
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
    $r = CRM_Core_DAO_AllCoreTables::getExports(__CLASS__, 'cxn', $prefix, []);
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
      'UI_appid' => [
        'name' => 'UI_appid',
        'field' => [
          0 => 'app_guid',
        ],
        'localizable' => FALSE,
        'unique' => TRUE,
        'sig' => 'civicrm_cxn::1::app_guid',
      ],
      'UI_keypair_cxnid' => [
        'name' => 'UI_keypair_cxnid',
        'field' => [
          0 => 'cxn_guid',
        ],
        'localizable' => FALSE,
        'unique' => TRUE,
        'sig' => 'civicrm_cxn::1::cxn_guid',
      ],
    ];
    return ($localize && !empty($indices)) ? CRM_Core_DAO_AllCoreTables::multilingualize(__CLASS__, $indices) : $indices;
  }

}
