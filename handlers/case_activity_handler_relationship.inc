<?php

class case_activity_handler_relationship extends views_handler_relationship {
  static $_activityTypes;

  /**
   * Init handler to let relationships live on tables other than
   * the table they operate on.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
    
    /*$activity_type_group = civicrm_api3('OptionGroup', 'getvalue', array('return' => 'id', 'name' => 'activity_type'));
    $activities = civicrm_api3('OptionValue', 'get', array('option_group_id' => $activity_type_group));*/
    $activities = civicrm_api3('Activity', 'getoptions', array('field' => 'activity_type'));
    
    // relationshipType() returns information about relations as array with fields
    // 'name_a_b', 'name_b_a', 'contact_type_a' and 'contact_type_b'.
    $options = array(0 => 'Any');
    foreach ($activities['values'] as $key => $value) {
      $options[$key] = $value;
    }

    self::$_activityTypes = $options;
  }

  /**
   * Get this field's label.
   */
  function label() {
    if (!isset($this->options['label'])) {
      return '';
    }
    return $this->options['label'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $label = !empty($this->definition['label']) ? $this->definition['label'] : $this->definition['title'];
    $options['label'] = array('default' => $label, 'translatable' => TRUE);
    $options['activity_type'] = array('default' => 0);
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Identifier'),
      '#default_value' => isset($this->options['label']) ? $this->options['label'] : $this->definition['label'],
      '#description' => t('Edit the administrative label displayed when referencing this relationship form filters, etc.'),
    );

    $form['activity_type'] = array(
      '#type' => 'select',
      '#title' => 'Choose a specific activity type',
      '#options' => self::$_activityTypes,
      '#description' => t('Use this drop down to limit this relatioship to a specific type of CiviCRM activities'),
      '#default_value' => $this->options['activity_type'],
    );
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    parent::query();

    // Add a join condition to the on clause to narrow down the relationship type shown
    if (isset($this->options['activity_type']) && $this->options['activity_type']) {
      $this->query->table_queue[$this->alias]['join']->extra[] = array(
        'value' => $this->options['activity_type'],
        'numeric' => TRUE,
        'field' => 'activity_type_id',
        'operator' => '=',
      );
    }
  }
}

