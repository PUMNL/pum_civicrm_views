<?php

/**
 * This relationship handler is used when joining the civicrm_relationship table
 * to the civicrm_contact table. This handler allows us to optionally add conditions
 * to the join clause based on relationship_type_id and is_active.
 */
class pum_civicrm_handler_join_multi_relationship extends views_handler_relationship {
  static $relationship_types;

  /**
   * Preload the list of relationship_types and store in the static variable
   * $relationship_types
   */
  function construct() {
    parent::construct();

    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Core/PseudoConstant.php';

    // relationshipType() returns information about relations as array with fields
    // 'name_a_b', 'name_b_a', 'contact_type_a' and 'contact_type_b'.
    $options = array(0 => 'Any');
    foreach (CRM_Core_PseudoConstant::relationshipType('label') as $id => $info) {
      $options[$id] = "{$info['label_a_b']} | {$info['label_b_a']}";
    }
    self::$relationship_types = $options;
  }

  /**
   * Add additional options for relationship_type and relationship_state
   * to the view. By defining these here, Views will take care of saving the
   * values submitted from the options form.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['relationship_type'] = array('default' => 0);
    $options['relationship_state'] = array('default' => 1);
    return $options;
  }

  /**
   * Add relationship_type drowndown and relationship_state checkbox to
   * relationship configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['relationship_type'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => 'Choose a specific relationship type',
      '#options' => self::$relationship_types,
      '#description' => t('Use this drop down to limit this relationship to specific type(s) of CiviCRM relationship'),
      '#default_value' => $this->options['relationship_type'],
    );
    $form['relationship_state'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit results only to active relationships?'),
      '#default_value' => $this->options['relationship_state'],
      '#description' => t('Exclude relationships that are inactive.')
    );
  }

  /**
   * Modify the default views relationship query to optionally specify
   * join conditions for relationship_type or is_active (relationship_state).
   */
  function query() {
    parent::query();
    // Specify the type of relationships to join
    $this->query->table_queue[$this->alias]['join']->extra[] = array(
      'value' => $this->options['relationship_type'],
      'numeric' => TRUE,
      'field' => 'relationship_type_id',
      'operator' => 'IN',
    );
    // Specify the state of relationships to join
    if (isset($this->options['relationship_state']) && $this->options['relationship_state']) {
      $this->query->table_queue[$this->alias]['join']->extra[] = array(
        'value' => 1,
        'numeric' => TRUE,
        'field' => 'is_active',
        'operator' => '=',
      );
    }
  }
  function pum_log_message($message, $title) {
    $fileName = '/var/www/pum_acc/sites/default/files/civicrm/ConfigAndLog/hommel'.date('Ymdhis').'.log';
    $log = fopen($fileName, 'w');
    fputs($log, $title);
    fputs($log, "\n");
    foreach ($message as $message_key => $message_value) {
      if (is_array($message_value)) {
        fputs($log, 'message_key: '.$message_key.' with array message_value:');
        fputs($log, "\n");
        foreach ($message_value as $value_key => $value_value) {
          if (is_array($value_value)) {
            fputs($log, 'value_key: '.$value_key.' with value_value array: '.implode('; ', $value_value));
            fputs($log, "\n");
          } else {
            fputs($log, 'value_key: '.$value_key.' with value_value: '.$value_value);
            fputs($log, "\n");
          }
        }
      } else {
        fputs($log, 'message_key: '.$message_key.', message_value: '.$message_value);
        fputs($log, "\n");
      }
    }
    fclose($log);
  }
}