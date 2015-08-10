api = 2
core = 7.30

; ====== DIFFICULT =========

; CiviCRM Modules

projects[civicrm_certify][subdir] = "contrib-cm"
projects[civicrm_certify][version] = "2.0-alpha3"

projects[civicrm_event_auto_pass][subdir] = "contrib-cm"
projects[civicrm_event_auto_pass][version] = "1.x-dev"

projects[civicrm_prereq_check][subdir] = "contrib-cm"
projects[civicrm_prereq_check][version] = "1.x-dev"

; Community Media Modules
projects[cm_airing_grid][subdir] = "contrib-cm"
projects[cm_airing_grid][version] = "1.x-dev"

projects[reservations][subdir] = "contrib-cm"
projects[reservations][version] = "2.0-alpha2"

projects[reservations_inventory][subdir] = "contrib-cm"
projects[reservations_inventory][version] = "1.x-dev"

projects[reservations_auto_checkin_out][subdir] = "contrib-cm"
projects[reservations_auto_checkin_out][version] = "1.x-dev"

projects[telvue][subdir] = "contrib-cm"
projects[telvue][version] = "1.0-alpha5"

projects[cm_cablecast][subdir] = "contrib-cm"
projects[cm_cablecast][version] = "2.x-dev"

projects[cinegy][subdir] = "contrib-cm"
projects[cinegy][version] = "1.x-dev"

projects[morpheus][subdir] = "contrib-cm"
projects[morpheus][version] = "1.x-dev"

; Contrib Modules
projects[bakery][subdir] = "contrib"
projects[bakery][version] = "2.0-alpha4"

projects[diff][subdir] = "contrib"
projects[diff][version] = "3.2"

projects[field_sync][subdir] = "contrib"
projects[field_sync][version] = "1.0-beta1"

projects[filter_perms][subdir] = "contrib"
projects[filter_perms][version] = "1.0"

projects[views_data_export][subdir] = "contrib"
projects[views_data_export][version] = "3.0-beta7"

projects[views_autocomplete_filters][subdir] = "contrib"
projects[views_autocomplete_filters][version] = "1.1"

projects[userpoints][subdir] = "contrib"
projects[userpoints][version] = "1.0"

; ====== CIVICRM RELATED =========

libraries[civicrm][download][type] = get
libraries[civicrm][download][url] = "http://downloads.civicrm.org/civicrm-4.4.6-starterkit.tgz"
libraries[civicrm][destination] = modules
libraries[civicrm][directory_name] = civicrm

;PATCHES THAT CHANGED BETWEEN 4.3 and 4.4
libraries[civicrm][patch][1978142] = http://drupal.org/files/2082713-pantheon-settings-4-4-2.patch
;libraries[civicrm][patch][1978142] = http://drupal.org/files/1978142-pantheon-settings-12.patch

;PATCHES THAT SHOULD BE ADDRESSED IN FUTURE CIVICRM RELEASES
libraries[civicrm][patch][1844558] = https://www.drupal.org/files/issues/1844558-run-civicrm-from-profile-dir-config-3.patch
libraries[civicrm][patch][1940074] = http://drupal.org/files/1940074-openFlashChart_tpl_javascript-4.patch
libraries[civicrm][patch][1967972] = http://drupal.org/files/1967972-bootsrap-fixes.patch
libraries[civicrm][patch][2130213] = http://drupal.org/files/issues/2130213-ignore-timezone-on-install-2.patch
libraries[civicrm][patch][2153441] = https://drupal.org/files/issues/2153441_formatresourceurl-4.patch
libraries[civicrm][patch][] = https://drupal.org/files/issues/2153441_formatresourceurl-5.patch

;PANTHEON RELATED PATCHES
libraries[civicrm][patch][2063371] = http://drupal.org/files/2063371-add-modulePath-var-4-4.patch
libraries[civicrm][patch][1978796] = http://drupal.org/files/1978796-session.save-as_file.patch
libraries[civicrm][patch][1978838] = http://drupal.org/files/issues/1978838-pre-populate-db-settings-2.patch

;IMPROVING PROFILE INSTALL UX WHEN INSTALLING FROM A PROFILE
libraries[civicrm][patch][1849424] = https://drupal.org/files/issues/1849424-use-vars-in-link-2.patch
libraries[civicrm][patch][] = http://drupal.org/files/1849424-pass-vars-in-link-2.patch
libraries[civicrm][patch][2130213] = http://drupal.org/files/issues/2130213-ignore-timezone-on-install-2.patch

;PATCHES THAT ADD LIBRARIES BACK
libraries[jquery][download][type] = get
libraries[jquery][download][url] = "http://code.jquery.com/jquery-1.8.3.min.js"
libraries[jquery][destination] = "modules/civicrm/packages"
libraries[jquery][directory_name] = jquery
libraries[jquery][download][filename] = jquery-1.8.3.min.js
libraries[jquery][patch][1787976] = http://drupal.org/files/1787976-jquery-missing-files-13.patch
libraries[jquery][patch][] = http://drupal.org/files/1787976-updated-fo-4-3-3.patch
libraries[jquery][patch][] = http://drupal.org/files/textarearesizer-4.patch
; JQuery Notify and Validate were whitelisted
libraries[jquery][patch][1950068] = http://drupal.org/files/1950068-jquery-redirect.patch
libraries[jquery][patch][2018177] = http://drupal.org/files/2018177-jquery-formnavigate-js-2.patch

libraries[jquery_notify][download][type] = get
libraries[jquery_notify][download][url] = "https://raw.github.com/ehynds/jquery-notify/1.5/src/jquery.notify.js"
libraries[jquery_notify][download][filename] = jquery.notify.js
libraries[jquery_notify][directory_name] = plugins
libraries[jquery_notify][destination] = "modules/civicrm/packages/jquery"

libraries[jquery_notify_min][download][type] = get
libraries[jquery_notify_min][download][url] = "https://raw.github.com/ehynds/jquery-notify/1.5/src/jquery.notify.min.js"
libraries[jquery_notify_min][download][filename] = jquery.notify.min.js
libraries[jquery_notify_min][directory_name] = plugins
libraries[jquery_notify_min][destination] = "modules/civicrm/packages/jquery"

libraries[jquery_validate][download][type] = get
libraries[jquery_validate][download][url] = "https://raw.github.com/jzaefferer/jquery-validation/1.9.0/jquery.validate.js"
libraries[jquery_validate][download][filename] = jquery.validate.js
libraries[jquery_validate][directory_name] = plugins
libraries[jquery_validate][destination] = "modules/civicrm/packages/jquery"

libraries[jquery_validate_min][download][type] = get
libraries[jquery_validate_min][download][url] = "https://raw.github.com/jzaefferer/jquery-validation/1.9.0/jquery.validate.js"
libraries[jquery_validate_min][download][filename] = jquery.validate.min.js
libraries[jquery_validate_min][directory_name] = plugins
libraries[jquery_validate_min][destination] = "modules/civicrm/packages/jquery"

libraries[jquery_ui][download][type] = get
libraries[jquery_ui][download][url] = "http://jquery-ui.googlecode.com/files/jquery-ui-1.9.0-rc.1.zip"
libraries[jquery_ui][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0"
libraries[jquery_ui][directory_name] = development-bundle

; MANUALLY GRAB SPECIFIC IMAGES FOR JQUERY UI
libraries[jquery_ui_bg_flat_0][download][type] = get
libraries[jquery_ui_bg_flat_0][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_flat_0_aaaaaa_40x100.png"
libraries[jquery_ui_bg_flat_0][download][filename] = ui-bg_flat_0_aaaaaa_40x100.png
libraries[jquery_ui_bg_flat_0][directory_name] = images
libraries[jquery_ui_bg_flat_0][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_flat_75][download][type] = get
libraries[jquery_ui_bg_flat_75][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_flat_75_ffffff_40x100.png"
libraries[jquery_ui_bg_flat_75][download][filename] = ui-bg_flat_75_ffffff_40x100.png
libraries[jquery_ui_bg_flat_75][directory_name] = images
libraries[jquery_ui_bg_flat_75][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_glass_55][download][type] = get
libraries[jquery_ui_bg_glass_55][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_glass_55_fbf9ee_1x400.png"
libraries[jquery_ui_bg_glass_55][download][filename] = ui-bg_glass_55_fbf9ee_1x400.png
libraries[jquery_ui_bg_glass_55][directory_name] = images
libraries[jquery_ui_bg_glass_55][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_glass_65][download][type] = get
libraries[jquery_ui_bg_glass_65][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_glass_65_ffffff_1x400.png"
libraries[jquery_ui_bg_glass_65][download][filename] = ui-bg_glass_65_ffffff_1x400.png
libraries[jquery_ui_bg_glass_65][directory_name] = images
libraries[jquery_ui_bg_glass_65][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_glass_75_dadada][download][type] = get
libraries[jquery_ui_bg_glass_75_dadada][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_glass_75_dadada_1x400.png"
libraries[jquery_ui_bg_glass_75_dadada][download][filename] = ui-bg_glass_75_dadada_1x400.png
libraries[jquery_ui_bg_glass_75_dadada][directory_name] = images
libraries[jquery_ui_bg_glass_75_dadada][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_glass_75_e6e6e6][download][type] = get
libraries[jquery_ui_bg_glass_75_e6e6e6][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_glass_75_e6e6e6_1x400.png"
libraries[jquery_ui_bg_glass_75_e6e6e6][download][filename] = ui-bg_glass_75_e6e6e6_1x400.png
libraries[jquery_ui_bg_glass_75_e6e6e6][directory_name] = images
libraries[jquery_ui_bg_glass_75_e6e6e6][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_glass_95][download][type] = get
libraries[jquery_ui_bg_glass_95][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_glass_95_fef1ec_1x400.png"
libraries[jquery_ui_bg_glass_95][download][filename] = ui-bg_glass_95_fef1ec_1x400.png
libraries[jquery_ui_bg_glass_95][directory_name] = images
libraries[jquery_ui_bg_glass_95][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_bg_highlight][download][type] = get
libraries[jquery_ui_bg_highlight][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-bg_highlight-soft_75_cccccc_1x100.png"
libraries[jquery_ui_bg_highlight][download][filename] = ui-bg_highlight-soft_75_cccccc_1x100.png
libraries[jquery_ui_bg_highlight][directory_name] = images
libraries[jquery_ui_bg_highlight][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_icons_2e83ff][download][type] = get
libraries[jquery_ui_icons_2e83ff][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-icons_2e83ff_256x240.png"
libraries[jquery_ui_icons_2e83ff][download][filename] = ui-icons_2e83ff_256x240.png
libraries[jquery_ui_icons_2e83ff][directory_name] = images
libraries[jquery_ui_icons_2e83ff][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_icons_222222][download][type] = get
libraries[jquery_ui_icons_222222][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-icons_222222_256x240.png"
libraries[jquery_ui_icons_222222][download][filename] = ui-icons_222222_256x240.png
libraries[jquery_ui_icons_222222][directory_name] = images
libraries[jquery_ui_icons_222222][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_icons_454545][download][type] = get
libraries[jquery_ui_icons_454545][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-icons_454545_256x240.png"
libraries[jquery_ui_icons_454545][download][filename] = ui-icons_454545_256x240.png
libraries[jquery_ui_icons_454545][directory_name] = images
libraries[jquery_ui_icons_454545][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_icons_888888][download][type] = get
libraries[jquery_ui_icons_888888][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-icons_888888_256x240.png"
libraries[jquery_ui_icons_888888][download][filename] = ui-icons_888888_256x240.png
libraries[jquery_ui_icons_888888][directory_name] = images
libraries[jquery_ui_icons_888888][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"

libraries[jquery_ui_icons_cd0a0a][download][type] = get
libraries[jquery_ui_icons_cd0a0a][download][url] = "https://raw.github.com/jquery/jquery-ui/master/themes/base/images/ui-icons_cd0a0a_256x240.png"
libraries[jquery_ui_icons_cd0a0a][download][filename] = ui-icons_cd0a0a_256x240.png
libraries[jquery_ui_icons_cd0a0a][directory_name] = images
libraries[jquery_ui_icons_cd0a0a][destination] = "modules/civicrm/packages/jquery/jquery-ui-1.9.0/css/smoothness"


libraries[jstree][download][type] = get
libraries[jstree][download][url] = "https://github.com/vakata/jstree/archive/v.pre1.0.zip"
libraries[jstree][destination] = "modules/civicrm/packages/jquery/plugins"
libraries[jstree][directory_name] = jstree



; THIS IS INSANE!!!  But the alternative is adjust paths w/ patched to core CiviCRM files
; because we need just the files in /lib, this has to be done this way for 40 files in
; https://github.com/PHPIDS/PHPIDS/tree/0.6.5%2Bpl1/lib/IDS

libraries[phpids_converter][download][type] = get
libraries[phpids_converter][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Converter.php"
libraries[phpids_converter][download][filename] = Converter.php
libraries[phpids_converter][directory_name] = IDS
libraries[phpids_converter][destination] = "modules/civicrm/packages"

libraries[phpids_event][download][type] = get
libraries[phpids_event][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Event.php"
libraries[phpids_event][download][filename] = Event.php
libraries[phpids_event][directory_name] = IDS
libraries[phpids_event][destination] = "modules/civicrm/packages"

libraries[phpids_filter][download][type] = get
libraries[phpids_filter][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Filter.php"
libraries[phpids_filter][download][filename] = Filter.php
libraries[phpids_filter][directory_name] = IDS
libraries[phpids_filter][destination] = "modules/civicrm/packages"

libraries[phpids_init][download][type] = get
libraries[phpids_init][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Init.php"
libraries[phpids_init][download][filename] = Init.php
libraries[phpids_init][directory_name] = IDS
libraries[phpids_init][destination] = "modules/civicrm/packages"

libraries[phpids_monitor][download][type] = get
libraries[phpids_monitor][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Monitor.php"
libraries[phpids_monitor][download][filename] = Monitor.php
libraries[phpids_monitor][directory_name] = IDS
libraries[phpids_monitor][destination] = "modules/civicrm/packages"

libraries[phpids_report][download][type] = get
libraries[phpids_report][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Report.php"
libraries[phpids_report][download][filename] = Report.php
libraries[phpids_report][directory_name] = IDS
libraries[phpids_report][destination] = "modules/civicrm/packages"

libraries[phpids_version][download][type] = get
libraries[phpids_version][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Version.php"
libraries[phpids_version][download][filename] = Version.php
libraries[phpids_version][directory_name] = IDS
libraries[phpids_version][destination] = "modules/civicrm/packages"

libraries[phpids_default_filter_json][download][type] = get
libraries[phpids_default_filter_json][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/default_filter.json"
libraries[phpids_default_filter_json][download][filename] = default_filter.json
libraries[phpids_default_filter_json][directory_name] = IDS
libraries[phpids_default_filter_json][destination] = "modules/civicrm/packages"

libraries[phpids_default_filter_xml][download][type] = get
libraries[phpids_default_filter_xml][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/default_filter.xml"
libraries[phpids_default_filter_xml][download][filename] = default_filter.xml
libraries[phpids_default_filter_xml][directory_name] = IDS
libraries[phpids_default_filter_xml][destination] = "modules/civicrm/packages"

; Caching
libraries[phpids_apc][download][type] = get
libraries[phpids_apc][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Apc.php"
libraries[phpids_apc][download][filename] = Apc.php
libraries[phpids_apc][directory_name] = Caching
libraries[phpids_apc][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_database][download][type] = get
libraries[phpids_database][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Database.php"
libraries[phpids_database][download][filename] = Database.php
libraries[phpids_database][directory_name] = Caching
libraries[phpids_database][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_factory][download][type] = get
libraries[phpids_factory][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Factory.php"
libraries[phpids_factory][download][filename] = Factory.php
libraries[phpids_factory][directory_name] = Caching
libraries[phpids_factory][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_file][download][type] = get
libraries[phpids_file][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/File.php"
libraries[phpids_file][download][filename] = File.php
libraries[phpids_file][directory_name] = Caching
libraries[phpids_file][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_interface][download][type] = get
libraries[phpids_interface][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Interface.php"
libraries[phpids_interface][download][filename] = Interface.php
libraries[phpids_interface][directory_name] = Caching
libraries[phpids_interface][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_memcached][download][type] = get
libraries[phpids_memcached][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Memcached.php"
libraries[phpids_memcached][download][filename] = Memcached.php
libraries[phpids_memcached][directory_name] = Caching
libraries[phpids_memcached][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_session][download][type] = get
libraries[phpids_session][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Caching/Session.php"
libraries[phpids_session][download][filename] = Session.php
libraries[phpids_session][directory_name] = Caching
libraries[phpids_session][destination] = "modules/civicrm/packages/IDS"

; Config
;libraries[phpids_config][download][type] = get
;libraries[phpids_config][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Config/Config.ini.php"
;libraries[phpids_config][download][filename] = Config.ini.php
;libraries[phpids_config][directory_name] = Config
;libraries[phpids_config][destination] = "modules/civicrm/packages/IDS"

; Filter
libraries[phpids_filter_storage][download][type] = get
libraries[phpids_filter_storage][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Filter/Storage.php"
libraries[phpids_filter_storage][download][filename] = Storage.php
libraries[phpids_filter_storage][directory_name] = Filter
libraries[phpids_filter_storage][destination] = "modules/civicrm/packages/IDS"

; Log
libraries[phpids_log_composite][download][type] = get
libraries[phpids_log_composite][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Log/Composite.php"
libraries[phpids_log_composite][download][filename] = Composite.php
libraries[phpids_log_composite][directory_name] = Log
libraries[phpids_log_composite][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_log_database][download][type] = get
libraries[phpids_log_database][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Log/Database.php"
libraries[phpids_log_database][download][filename] = Database.php
libraries[phpids_log_database][directory_name] = Log
libraries[phpids_log_database][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_log_email][download][type] = get
libraries[phpids_log_email][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Log/Email.php"
libraries[phpids_log_email][download][filename] = Email.php
libraries[phpids_log_email][directory_name] = Log
libraries[phpids_log_email][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_log_file][download][type] = get
libraries[phpids_log_file][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Log/File.php"
libraries[phpids_log_file][download][filename] = File.php
libraries[phpids_log_file][directory_name] = Log
libraries[phpids_log_file][destination] = "modules/civicrm/packages/IDS"

libraries[phpids_log_interface][download][type] = get
libraries[phpids_log_interface][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/0.6.5%2Bpl1/lib/IDS/Log/Interface.php"
libraries[phpids_log_interface][download][filename] = Interface.php
libraries[phpids_log_interface][directory_name] = Log
libraries[phpids_log_interface][destination] = "modules/civicrm/packages/IDS"

; This file is added to create the sites/all/extensions directory
libraries[cache][download][type] = get
libraries[cache][download][url] = "https://raw.github.com/PHPIDS/PHPIDS/master/README.md"
libraries[cache][download][filename] = timestamp.txt
libraries[cache][destination] = extensions
libraries[cache][patch][1980088] = https://drupal.org/files/1980088-create-extensions-dir-4.patch

libraries[htmlpurifier][download][type] = get
libraries[htmlpurifier][download][url] = "http://htmlpurifier.org/releases/htmlpurifier-4.6.0.tar.gz"
libraries[htmlpurifier][destination] = "modules/civicrm/packages/IDS/vendors"
libraries[htmlpurifier][directory_name] = htmlpurifier

libraries[dompdf][download][type] = get
libraries[dompdf][download][url] = "http://dompdf.googlecode.com/files/dompdf_0-6-0_beta3.tar.gz"
libraries[dompdf][destination] = "modules/civicrm/packages"
libraries[dompdf][directory_name] = dompdf

; CKEDitor and TinyMCE are included twice... really need to change that
; with a patch that allows CiviCRM to use sites/all/libraries

libraries[ckeditor-civicrm][download][type] = get
libraries[ckeditor-civicrm][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%203.6.2/ckeditor_3.6.2.tar.gz"
libraries[ckeditor-civicrm][destination] = "modules/civicrm/packages"
libraries[ckeditor-civicrm][directory_name] = ckeditor

libraries[tinymce-civicrm][download][type] = get
libraries[tinymce-civicrm][download][url] = "http://download.moxiecode.com/tinymce/tinymce_3.5.11.zip"
libraries[tinymce-civicrm][destination] = "modules/civicrm/packages"
libraries[tinymce-civicrm][directory_name] = tinymce

; ====== MODERATE =========

; CiviCRM Modules
projects[civicrm_clear_all_caches][subdir] = "contrib"
projects[civicrm_clear_all_caches][version] = "1.0-beta1"

projects[civicrm_cron][subdir] = "contrib"
projects[civicrm_cron][version] = "2.0-beta2"

projects[civicrm_multiday_event][subdir] = "contrib-cm"
projects[civicrm_multiday_event][version] = "1.0-rc1"

projects[civicrm_user_reference][subdir] = "contrib-cm"
projects[civicrm_user_reference][version] = "1.x-dev"

projects[civicrm_realname][subdir] = "contrib"
projects[civicrm_realname][version] = "1.0"

projects[options_element][subdir] = "contrib"
projects[options_element][version] = "1.12"

projects[webform_civicrm][subdir] = "contrib"
projects[webform_civicrm][version] = "4.7"

; Community Media Modules
projects[cm_project][subdir] = "contrib-cm"
projects[cm_project][version] = "2.0-beta1"

projects[cm_profile][subdir] = "contrib-cm"
projects[cm_profile][version] = "1.x-dev"

projects[jupload][subdir] = "contrib-cm"
projects[jupload][version] = "1.x-dev"

; Contrib Modules
projects[auto_entitylabel][subdir] = "contrib"
projects[auto_entitylabel][version] = "1.2"

projects[colors][subdir] = "contrib"
projects[colors][version] = "1.0-rc1"

projects[context][subdir] = "contrib"
projects[context][version] = "3.2"

projects[devel][subdir] = "contrib"
projects[devel][version] = "1.5"

projects[delta][subdir] = "contrib"
projects[delta][version] = "3.0-beta11"

projects[environment_indicator][subdir] = "contrib"
projects[environment_indicator][version] = "2.5"

projects[field_collection][subdir] = "contrib"
projects[field_collection][version] = "1.0-beta7"

projects[field_permissions][subdir] = "contrib"
projects[field_permissions][version] = "1.0-beta2"

projects[flag][subdir] = "contrib"
projects[flag][version] = "3.5"

projects[hms_field][subdir] = "contrib"
projects[hms_field][version] = "1.2"

projects[link][subdir] = "contrib"
projects[link][version] = "1.2"

projects[og][subdir] = "contrib"
projects[og][version] = "2.7"

projects[og_extras][subdir] = "contrib"
projects[og_extras][version] = "1.1"

projects[realname][subdir] = "contrib"
projects[realname][version] = "1.1"

projects[socialmedia][subdir] = "contrib"
projects[socialmedia][version] = "1.0-beta13"

projects[views_accordion][subdir] = "contrib"
projects[views_accordion][version] = "1.0"

; ====== EASY ==========

; Community Media Modules
projects[cm_airing][subdir] = "contrib-cm"
projects[cm_airing][version] = "3.0-alpha3"

projects[cm_checklist][subdir] = "contrib-cm"
projects[cm_checklist][version] = "1.x-dev"

projects[cm_header][subdir] = "contrib-cm"
projects[cm_header][version] = "1.0-alpha1"

projects[cm_show][subdir] = "contrib-cm"
projects[cm_show][version] = "1.0-beta1"

projects[cm_show_vod][subdir] = "contrib-cm"
projects[cm_show_vod][version] = "2.x-dev"

projects[cm_slideshow][subdir] = "contrib-cm"
projects[cm_slideshow][version] = "2.0-alpha1"

projects[cm_vod_feed][subdir] = "contrib-cm"
projects[cm_vod_feed][version] = "2.0-beta1"

projects[om_crew_connect][subdir] = "contrib-cm"
projects[om_crew_connect][version] = "2.0-beta4"

; Contribb Modules
projects[backup_migrate][subdir] = "contrib"
projects[backup_migrate][version] = "2.8"

projects[badbot][subdir] = "contrib"
projects[badbot][version] = "1.1"

projects[block_class][subdir] = "contrib"
projects[block_class][version] = "2.1"

projects[breakpoints][subdir] = "contrib"
projects[breakpoints][version] = "1.1"

projects[colors][subdir] = "contrib"
projects[colors][version] = "1.0-rc1"

projects[commentsblock][subdir] = "contrib"
projects[commentsblock][version] = "2.2"

projects[css_injector][subdir] = "contrib"
projects[css_injector][version] = "1.8"

projects[ctools][subdir] = "contrib"
projects[ctools][version] = "1.4"

projects[captcha][subdir] = "contrib"
projects[captcha][version] = "1.0"

projects[creativecommons][subdir] = "contrib"
projects[creativecommons][version] = "2.x-dev"

projects[date][subdir] = "contrib"
projects[date][version] = "2.6"

projects[draggable_captcha][subdir] = "contrib"
projects[draggable_captcha][version] = "1.2"

projects[empty_page][subdir] = "contrib"
projects[empty_page][version] = "1.0"

projects[entity][subdir] = "contrib"
projects[entity][version] = "1.3"  

projects[entityreference][subdir] = "contrib"
projects[entityreference][version] = "1.x-dev"

projects[features][subdir] = "contrib"
projects[features][version] = "2.0"

projects[feeds][subdir] = "contrib"
projects[feeds][version] = "2.x-dev"

projects[feeds_cablecast][subdir] = "contrib"
projects[feeds_cablecast][version] = "1.x-dev"

projects[feeds_mediarss][subdir] = "contrib"
projects[feeds_mediarss][version] = "1.x-dev"

projects[feeds_media_internet_files][subdir] = "contrib"
projects[feeds_media_internet_files][version] = "1.x-dev"

projects[feeds_telvue][subdir] = "contrib"
projects[feeds_telvue][version] = "1.x-dev"

projects[field_group][subdir] = "contrib"
projects[field_group][version] = "1.3"

projects[file_entity][subdir] = "contrib"
projects[file_entity][version] = "2.0-alpha3"

projects[fitvids][subdir] = "contrib"
projects[fitvids][version] = "1.14"

projects[flexslider][subdir] = "contrib"
projects[flexslider][version] = "2.0-alpha3"  

projects[flexslider_views_slideshow][subdir] = "contrib"
projects[flexslider_views_slideshow][version] = "2.x-dev" 

projects[fontyourface][subdir] = "contrib"
projects[fontyourface][version] = "2.8"

projects[fullcalendar][subdir] = "contrib"
projects[fullcalendar][version] = "2.0"

projects[headerimage][subdir] = "contrib"
projects[headerimage][version] = "1.4"

projects[hms_field][subdir] = "contrib"
projects[hms_field][version] = "1.x-dev"
projects[hms_field][patch][2119623] = "http://drupal.org/files/2119623-hidden_field_empty_error.patch"

projects[google_analytics][subdir] = "contrib"
projects[google_analytics][version] = "1.4"

projects[imce][subdir] = "contrib"
projects[imce][version] = "1.7"

projects[imce_wysiwyg][subdir] = "contrib"
projects[imce_wysiwyg][version] = "1.0"
projects[imce_wysiwyg][patch][1794930] = "http://drupal.org/files/imce_wysiwyg-access-issue-1825850-2.patch"

projects[job_scheduler][subdir] = "contrib"
projects[job_scheduler][version] = "2.0-alpha3"

projects[jquery_update][subdir] = "contrib"
projects[jquery_update][version] = "2.3"

projects[legal][subdir] = "contrib"
projects[legal][version] = "1.x-dev"

projects[libraries][subdir] = "contrib"
projects[libraries][version] = "2.1"

projects[media][subdir] = "contrib"
projects[media][version] = "2.0-alpha3"

projects[media_youtube][subdir] = "contrib"
projects[media_youtube][version] = "2.0-rc4"

projects[media_vimeo][subdir] = "contrib"
projects[media_vimeo][version] = "2.x-dev"

projects[media_bliptv][subdir] = "contrib"
projects[media_bliptv][version] = "1.x-dev"

projects[media_archive][subdir] = "contrib"
projects[media_archive][version] = "1.x-dev"

projects[media_cloudcast][subdir] = "contrib"
projects[media_cloudcast][version] = "2.0-beta1"

projects[menu_expanded][subdir] = "contrib"
projects[menu_expanded][version] = "1.0-beta1"

projects[module_filter][subdir] = "contrib"
projects[module_filter][version] = "1.8"

projects[menu_attributes][subdir] = "contrib"
projects[menu_attributes][version] = "1.0-rc2"

projects[options_element][subdir] = "office_hours"
projects[options_element][version] = "1.3"

projects[options_element][subdir] = "contrib"
projects[options_element][version] = "1.10"

projects[pathauto][subdir] = "contrib"
projects[pathauto][version] = "1.2"

projects[pbcore][subdir] = "contrib"
projects[pbcore][version] = "1.0-beta3"

projects[picture][subdir] = "contrib"
projects[picture][version] = "2.4"

projects[profile2][subdir] = "contrib"
projects[profile2][version] = "1.3"

projects[profile_status_check][subdir] = "contrib"
projects[profile_status_check][version] = "1.0-beta1"

projects[profile_switcher][subdir] = "contrib"
projects[profile_switcher][version] = "1.0-beta1"

projects[recaptcha][subdir] = "contrib"
projects[recaptcha][version] = "1.10"

projects[strongarm][subdir] = "contrib"
projects[strongarm][version] = "2.0"

projects[token][subdir] = "contrib"
projects[token][version] = "1.5"

projects[views][subdir] = "contrib"
projects[views][version] = "3.8"

projects[views_bulk_operations][subdir] = "contrib"
projects[views_bulk_operations][version] = "3.1"

projects[views_send][subdir] = "contrib"
projects[views_send][version] = "1.0-rc3"

projects[views_slideshow][subdir] = "contrib"
projects[views_slideshow][version] = "3.1"

projects[webform][subdir] = "contrib"
projects[webform][version] = "4.0-beta2"

projects[wysiwyg][subdir] = "contrib"
projects[wysiwyg][version] = "2.2"

; Themes
projects[omega][version] = "3.1"
projects[zen][version] = "5.4"
projects[cm_theme][version] = "2.0-beta1"

;Libraries
libraries[ckeditor][download][type] = get
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%203.6.2/ckeditor_3.6.2.tar.gz"
libraries[ckeditor][destination] = libraries
libraries[ckeditor][directory_name] = ckeditor

libraries[tinymce][download][type] = get
libraries[tinymce][download][url] = "http://download.moxiecode.com/tinymce/tinymce_3.5.11.zip"
libraries[tinymce][destination] = libraries
libraries[tinymce][directory_name] = tinymce

libraries[flexslider][download][type] = get
libraries[flexslider][download][url] = "https://github.com/woothemes/FlexSlider/archive/version/2.1.zip"
libraries[flexslider][destination] = libraries
libraries[flexslider][directory_name] = flexslider

libraries[fitvids][download][type] = get
libraries[fitvids][download][url] = "https://raw.github.com/davatron5000/FitVids.js/master/jquery.fitvids.js"
libraries[fitvids][destination] = libraries
libraries[fitvids][directory_name] = fitvids

libraries[fullcalendar][download][type] = get
libraries[fullcalendar][download][url] = "http://arshaw.com/fullcalendar/downloads/fullcalendar-1.5.3.zip"
libraries[fullcalendar][destination] = libraries
libraries[fullcalendar][directory_name] = fullcalendar-download