<?php
/**
 * Just provide some airing info for the entity administration page.
 * This can be expanded to add more information / a list of all created entites, etc.
 */
function certification_rule_admin_page() {
  $output = 'Welcome to the administration page for your Certification Type Entities.<br/>';
  $output .= '<ul class="action-links"><li><a href="/certification-rule/add">Add Certification Type</a></li></ul>';
  return $output;
}
