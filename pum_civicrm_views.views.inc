<?php

function pum_civicrm_views_views_data_alter(&$data) {
  _pum_civicrm_views_relationship_country($data);
  _pum_civicrm_views_relationship_relationship_by_case($data);
  _pum_civicrm_views_parent_case($data);
}

/**
 * Add a view relationship for civicrm relationship starting from a case
 */
function _pum_civicrm_views_relationship_relationship_by_case(&$data) {
  $data['civicrm_case']['relationship_id'] = array(
  'real field' => 'id',
    'title' => t('CiviCRM Relationship (starting from contact case ID)'),
    'help' => t('Connects a case to a relationship.'),
    'relationship' => array(
      'base' => 'civicrm_relationship',
      'base field' => 'case_id',
      'handler' => 'civicrm_handler_relationship_relationship',
      'label' => t('CiviCRM Relationship (starting from case ID)'),
    ),
  );  
  
}

/**
 * Add view relation between CiviCRM Address and Contact
 * Between CiviCRM Address and Custom field civicrm_country_id of a country contact
 * and vice versa
 */

function _pum_civicrm_views_relationship_country(&$data) {
  $pumCountry = civicrm_api3('CustomGroup', 'getsingle', array('name' => 'pumCountry'));
  $pumCountryField = civicrm_api3('CustomField', 'getsingle', array('custom_group_id' => $pumCountry['id'], 'name' => 'civicrm_country_id'));
  $pumCountryTable = $pumCountry['table_name'];
  $pumCountryFieldName = $pumCountryField['column_name'];
  
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['base'] = 'civicrm_address';
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['base field'] = 'country_id';
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['handler'] = 'views_handler_relationship';
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['label'] = t('Related address');
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['title'] = t('Address country ID');
  $data[$pumCountryTable][$pumCountryFieldName]['relationship']['help'] = t('This link an civicrm address to a custom field country entry');
  
  $data[$pumCountryTable]['entity_id']['relationship']['base'] = 'civicrm_contact';
  $data[$pumCountryTable]['entity_id']['relationship']['base field'] = 'id';
  $data[$pumCountryTable]['entity_id']['relationship']['handler'] = 'views_handler_relationship';
  $data[$pumCountryTable]['entity_id']['relationship']['label'] = t('Country contact');
  $data[$pumCountryTable]['entity_id']['relationship']['title'] = t('Country Contact');
  
  //relationship from address to ccountry entry
  $data['civicrm_address']['country_id']['relationship']['base'] = $pumCountryTable;
  $data['civicrm_address']['country_id']['relationship']['base field'] = $pumCountryFieldName;
  $data['civicrm_address']['country_id']['relationship']['handler'] = 'pum_civicrm_views_primary_address_handler_relationship';
  $data['civicrm_address']['country_id']['relationship']['label'] = t('country_entry');
  $data['civicrm_address']['country_id']['relationship']['title'] = t('Country entry');
  
  //relationship from address to contact
  $data['civicrm_address']['contact_id']['relationship']['base'] = 'civicrm_contact';
  $data['civicrm_address']['contact_id']['relationship']['base field'] = 'id';
  $data['civicrm_address']['contact_id']['relationship']['handler'] = 'views_handler_relationship';
  $data['civicrm_address']['contact_id']['relationship']['label'] = t('Related address');
  $data['civicrm_address']['contact_id']['relationship']['title'] = t('Contact ID');
}

function _pum_civicrm_views_parent_case(&$data) {
  $link_case_to = civicrm_api3('CustomGroup', 'getsingle', array('name' => 'travel_parent'));
  $case_id = civicrm_api3('CustomField', 'getsingle', array('name' => 'case_id', 'custom_group_id' => $link_case_to['id']));

  $table = $link_case_to['table_name'];
  $field = $case_id['column_name'];

  $data[$table]['table']['group'] = t('CiviCRM Parent case');
  $data[$table]['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Parent case'),
    'help' => t('CiviCRM Parent case'),
    'weight' => -10,
  );

  $data[$table]['table']['join']['entity_id'] = array(
    'left_table' => 'civicrm_case',
    'left_field' => 'id',
    'field' => 'entity_id',
  );

  $data[$table]['table']['join'][$field] = array(
    'left_table' => 'civicrm_case',
    'left_field' => 'id',
    'field' => $field,
  );

  $data[$table]['entity_id'] = array(
    'title' => t('Child case'),
    'relationship' => array(
      'base' => 'civicrm_case',
      'left_field' => 'id',
      'field' => 'entity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Case (child)'),
    ),
  );

  $data[$table][$field] = array(
    'title' => t('Parent case'),
    'relationship' => array(
      'base' => 'civicrm_case',
      'left_field' => 'id',
      'field' => $field,
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Case (parent)'),
    ),
  );

  /*
  //relationship from address to ccountry entry
  $data['civicrm_address']['country_id']['relationship']['base'] = $pumCountryTable;
  $data['civicrm_address']['country_id']['relationship']['base field'] = $pumCountryFieldName;
  $data['civicrm_address']['country_id']['relationship']['handler'] = 'pum_civicrm_views_primary_address_handler_relationship';
  $data['civicrm_address']['country_id']['relationship']['label'] = t('country_entry');
  $data['civicrm_address']['country_id']['relationship']['title'] = t('Country entry');

  //relationship from address to contact
  $data['civicrm_address']['contact_id']['relationship']['base'] = 'civicrm_contact';
  $data['civicrm_address']['contact_id']['relationship']['base field'] = 'id';
  $data['civicrm_address']['contact_id']['relationship']['handler'] = 'views_handler_relationship';
  $data['civicrm_address']['contact_id']['relationship']['label'] = t('Related address');
  $data['civicrm_address']['contact_id']['relationship']['title'] = t('Contact ID');*/
}