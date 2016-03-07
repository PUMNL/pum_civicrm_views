<?php

/**
 * This relationship handlers makes sure the primary address is related in the relation
 * 
 */
class pum_civicrm_views_primary_address_handler_relationship extends views_handler_relationship {

  /**
   * Modify the default views relationship query to optionally specify
   * join conditions for relationship_type or is_active (relationship_state).
   */
  function query() {
    parent::query();
    $this->query->table_queue[$this->alias]['join']->extra[] = array(
      'value' => 1,
      'numeric' => TRUE,
      'field' => 'is_primary',
      'table' => 'civicrm_address',
      'operator' => '=',
    );
  }
}