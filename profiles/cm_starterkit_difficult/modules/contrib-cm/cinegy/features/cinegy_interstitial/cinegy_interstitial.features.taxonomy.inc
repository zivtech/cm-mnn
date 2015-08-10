<?php
/**
 * @file
 * cinegy_interstitial.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function cinegy_interstitial_taxonomy_default_vocabularies() {
  return array(
    'cinegy_type_template' => array(
      'name' => 'Cinegy Type template',
      'machine_name' => 'cinegy_type_template',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
