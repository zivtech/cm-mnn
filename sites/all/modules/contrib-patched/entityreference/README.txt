DESCRIPTION
===========
Provides a field type that can reference arbitrary entities.

SITE BUILDERS
=============
Note that when using a select widget, Entity reference loads all the
entities in that list in order to get the entity's label. If there are
too many loaded entities that site might reach its memory limit and crash
(also known as WSOD). In such a case you are advised to change the widget
to "autocomplete". If you get a WSOD when trying to edit the field
settings, you can reach the widget settings directly by navigation to

  admin/structure/types/manage/[ENTITY-TYPE]/fields/[FIELD-NAME]/widget-type

Replace ENTITY-TYPE and FIELD_NAME with the correct values.

Patches
-------
Mark Libkuman: patched to store global valid IDs that can override those used in
  the og module
  stores a global variable $hacked_valid_ids with ids so that id with a value
  can be used in og/plugins/entityreference/behavior/OgBehaviorHandler.class.php
  where this function is returning an empty array when it shouldn't be, 
  $valid_ids = entityreference_get_selection_handler($field, $instance, 
  $entity_type, $entity)->validateReferencableEntities($ids);
  
