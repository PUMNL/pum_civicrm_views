<?php

function pum_civicrm_views_views_data_alter(&$data) {
  
  $pumCountry = civicrm_api3('CustomGroup', 'getsingle', array('name' => 'pumCountry'));
  $pumCountryField = civicrm_api3('CustomField', 'getsingle', array('custom_group_id' => $pumCountry['id'], 'name' => 'civicrm_country_id'));
  $pumCountryTable = $pumCountry['table_name'];
  $pumCountryFieldName = $pumCountryField['column_name'];
  $customName = 'custom_'.$pumCountryField['id'];
  
  //var_dump($data['$pumCountryTable']); exit();
  //link civicrm_contact.custom_xx to civicrm_address.country_id
  $data[$pumCountryTable]['table']['join']['civicrm_address']['left_field'] = 'country_id';
  $data[$pumCountryTable]['table']['join']['civicrm_address']['field'] = $pumCountryFieldName;
  
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
  $data['civicrm_address']['country_id']['relationship']['handler'] = 'views_handler_relationship';
  $data['civicrm_address']['country_id']['relationship']['label'] = t('country_entry');
  $data['civicrm_address']['country_id']['relationship']['title'] = t('Country entry');
  
  //relationship from address to contact
  $data['civicrm_address']['contact_id']['relationship']['base'] = 'civicrm_contact';
  $data['civicrm_address']['contact_id']['relationship']['base field'] = 'id';
  $data['civicrm_address']['contact_id']['relationship']['handler'] = 'views_handler_relationship';
  $data['civicrm_address']['contact_id']['relationship']['label'] = t('Related address');
  $data['civicrm_address']['contact_id']['relationship']['title'] = t('Contact ID');
  
}