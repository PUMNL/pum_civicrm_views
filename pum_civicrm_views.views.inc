<?php

function pum_civicrm_views_views_data_alter(&$data) {
  _pum_civicrm_views_option_group($data);
  _pum_civicrm_views_relationship_country($data);
  _pum_civicrm_views_relationship_relationship_by_case($data);
  _pum_civicrm_views_parent_case($data);
  _pum_civicrm_views_projects($data);
  _pum_civicrm_views_dsa_compose($data);
  _pum_civicrm_views_representative_compose($data);
  _pum_civicrm_views_case($data);
}

/**
 * Add a sorting handler for case status
 */
function _pum_civicrm_views_case(&$data) {
  $data['civicrm_case']['status']['sort']['handler'] = 'pum_civicrm_handler_sort_case_status';
}

function _pum_civicrm_views_option_group(&$data) {
  $data['civicrm_option_group']['table']['group'] = t('CiviCRM Option Group');
  $data['civicrm_option_group']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Option Group'),
    'help' => t('CiviCRM Option Group'),
    'weight' => -10,
  );

  $data['civicrm_option_group']['table']['join']['id'] = array(
    'left_table' => 'civicrm_option_value',
    'left_field' => 'option_group_id',
    'field' => 'id',
  );

  $data['civicrm_option_group']['id'] = array(
    'title' => t('Option Group ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_option_group']['name'] = array(
    'title' => t('Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_option_value']['table']['group'] = t('CiviCRM Option Value');
  $data['civicrm_option_value']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Option Value'),
    'help' => t('CiviCRM Option Value'),
    'weight' => -10,
  );

  $data['civicrm_option_value']['table']['join']['option_group_id'] = array(
    'left_table' => 'civicrm_option_group',
    'left_field' => 'id',
    'field' => 'option_group_id',
  );

  $data['civicrm_option_value']['id'] = array(
    'title' => t('Option Value ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_option_value']['option_group_id'] = array(
    'title' => t('Option group ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'base' => 'civicrm_option_group',
      'left_field' => 'id',
      'field' => 'option_group_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Option Group'),
    ),
  );

  $data['civicrm_option_group']['value'] = array(
    'title' => t('Value'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
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

/**
 * Add relationship between cases through the parent case custom field set
 */
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
}


/**
 * Add the threepeas projects to the view
 */
function _pum_civicrm_views_dsa_compose(&$data) {
  $data['civicrm_dsa_compose']['table']['group'] = t('CiviCRM DSA Compose');
  $data['civicrm_dsa_compose']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM DSA Compose'),
    'help' => t('CiviCRM DSA Compose'),
    'weight' => -10,
  );

  $data['civicrm_dsa_compose']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_activity',
    'left_field' => 'id',
    'field' => 'activity_id',
    'handler' => 'pum_civicrm_handler_join_dsa_compose',
  );

  $data['civicrm_activity']['table']['join']['civicrm_dsa_compose'] = array(
    'left_table' => 'civicrm_dsa_compose',
    'left_field' => 'activity_id',
    'field' => 'id',
  );

  $data['civicrm_dsa_compose']['id'] = array(
    'title' => t('ID'),
    'relationship' => array(
      'base' => 'civicrm_activity',
      'left_field' => 'id',
      'field' => 'activity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM DSA Compose'),
    ),
  );

  $data['civicrm_dsa_compose']['activity_id'] = array(
    'title' => t('Activity'),
    'relationship' => array(
      'base' => 'civicrm_activity',
      'left_field' => 'id',
      'field' => 'activity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Activity'),
    ),
  );

  $data['civicrm_dsa_compose']['total_amount'] = array(
    'title' => t('Total amount'),
    'field' => array(
      'handler' => 'pum_civicrm_handler_dsa_compose_total_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_dsa'] = array(
    'title' => t('Amount DSA'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_briefing'] = array(
    'title' => t('Amount Briefing'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_airport'] = array(
    'title' => t('Amount Airport'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_transfer'] = array(
    'title' => t('Amount Transfer'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_hotel'] = array(
    'title' => t('Amount Hotel'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_visa'] = array(
    'title' => t('Amount Visa'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_medical'] = array(
    'title' => t('Amount Medical'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['amount_other'] = array(
    'title' => t('Amount Other'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_dsa_compose']['description_other'] = array(
    'title' => t('Description Other'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_dsa_compose']['amount_advance'] = array(
    'title' => t('Amount Advance'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  
  $data['civicrm_dsa_compose']['type'] = array(
    'title' => t('Type'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  //money handler to dsa field
  $bdsa_cg = civicrm_api3('CustomGroup', 'getsingle', array('name' => 'pum_business_dsa'));
  $bdsa_table = $bdsa_cg['table_name'];
  $bdsa_amount_field = civicrm_api3('CustomField', 'getvalue', array('name' => 'bdsa_amount', 'return' => 'column_name', 'custom_group_id' => $bdsa_cg['id']));
  $data[$bdsa_table][$bdsa_amount_field]['field']['handler'] = 'civicrm_handler_field_money';
}

/**
 * Add the representative payment to drupal views
 */
function _pum_civicrm_views_representative_compose(&$data) {
  $data['civicrm_representative_compose']['table']['group'] = t('CiviCRM Representative Compose');
  $data['civicrm_representative_compose']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Representative Compose'),
    'help' => t('CiviCRM Representative Compose'),
    'weight' => -10,
  );

  $data['civicrm_representative_compose']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_activity',
    'left_field' => 'id',
    'field' => 'activity_id',
    'handler' => 'pum_civicrm_handler_join_representative_compose',
  );

  $data['civicrm_representative_compose']['table']['join']['civicrm_contact'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  $data['civicrm_activity']['table']['join']['civicrm_representative_compose'] = array(
    'left_table' => 'civicrm_representative_compose',
    'left_field' => 'activity_id',
    'field' => 'id',
  );

  $data['civicrm_representative_compose']['id'] = array(
    'title' => t('ID'),
    'relationship' => array(
      'base' => 'civicrm_activity',
      'left_field' => 'id',
      'field' => 'activity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Representative Compose'),
    ),
  );

  $data['civicrm_representative_compose']['activity_id'] = array(
    'title' => t('Activity'),
    'relationship' => array(
      'base' => 'civicrm_activity',
      'left_field' => 'id',
      'field' => 'activity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Activity'),
    ),
  );

  $data['civicrm_representative_compose']['contact_id'] = array(
    'title' => t('Contact'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contact'),
    ),
  );

  $data['civicrm_representative_compose']['amount_rep'] = array(
    'title' => t('Total amount'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_representative_compose']['comments'] = array(
    'title' => t('Comments'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['civicrm_representative_compose']['type'] = array(
    'title' => t('Type'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
}


/**
 * Add the threepeas projects to the view
 */
function _pum_civicrm_views_projects(&$data) {
  $data['civicrm_project']['table']['group'] = t('CiviCRM Project');
  $data['civicrm_project']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Project'),
    'help' => t('CiviCRM Project'),
    'weight' => -10,
  );

  $data['civicrm_project']['table']['join']['customer_id'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'customer_id',
  );
  $data['civicrm_project']['table']['join']['civicrm_contact'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'customer_id',
  );
  $data['civicrm_project']['table']['join']['id'] = array(
    'left_table' => 'civicrm_case_project',
    'left_field' => 'project_id',
    'field' => 'id',
  );

  $data['civicrm_project']['id'] = array(
    'title' => t('Project ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['civicrm_project']['title'] = array(
    'title' => t('Title'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['reason'] = array(
    'title' => t('Reason'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['work_description'] = array(
    'title' => t('Work description'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['qualifications'] = array(
    'title' => t('Qualifications'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['expected_results'] = array(
    'title' => t('Expected results'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['projectplan'] = array(
    'title' => t('Project plan'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['civicrm_project']['customer_id'] = array(
    'title' => t('Customer'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'customer_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contact, with custom fields'),
    ),
  );

  $data['civicrm_project']['start_date'] = array(
    'title' => t('Start date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  $data['civicrm_project']['end_date'] = array(
    'title' => t('End date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  $data['civicrm_project']['is_active'] = array(
    'title' => t('Is active'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      // This setting is used by the boolean filter handler, as possible option.
      'type' => 'yes-no',
      // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment.
      'use equal' => true,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //case project table
  $data['civicrm_case_project']['table']['group'] = t('CiviCRM Case Project');
  $data['civicrm_case_project']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('CiviCRM Case Project'),
    'help' => t('CiviCRM Case Project link table'),
    'weight' => -10,
  );

  $data['civicrm_case_project']['table']['join']['civicrm_project'] = array(
    'left_table' => 'civicrm_project',
    'left_field' => 'id',
    'field' => 'project_id',
  );
  $data['civicrm_case_project']['table']['join']['project_id'] = array(
    'left_table' => 'civicrm_project',
    'left_field' => 'id',
    'field' => 'project_id',
  );
  $data['civicrm_case_project']['table']['join']['case_id'] = array(
    'left_table' => 'civicrm_case',
    'left_field' => 'id',
    'field' => 'case_id',
  );
  $data['civicrm_case_project']['table']['join']['civicrm_case'] = array(
    'left_table' => 'civicrm_case',
    'left_field' => 'id',
    'field' => 'case_id',
  );

  $data['civicrm_case_project']['project_id'] = array(
    'title' => t('Project ID'),
    'relationship' => array(
      'base' => 'civicrm_project',
      'left_field' => 'id',
      'field' => 'project_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Project'),
    ),
  );

  $data['civicrm_case_project']['case_id'] = array(
    'title' => t('Case ID'),
    'relationship' => array(
      'base' => 'civicrm_case',
      'left_field' => 'id',
      'field' => 'case_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Case'),
    ),
  );

  $data['civicrm_case_project']['is_active'] = array(
    'title' => t('Is active'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      // This setting is used by the boolean filter handler, as possible option.
      'type' => 'yes-no',
      // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment.
      'use equal' => true,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

}