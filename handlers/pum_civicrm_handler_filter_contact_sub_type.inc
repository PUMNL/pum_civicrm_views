<?php

class pum_civicrm_handler_filter_contact_sub_type extends civicrm_handler_filter_pseudo_constant {

  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $operator = 'LIKE';
    if ($this->operator == 'not in') {
      $operator = 'NOT LIKE';
    }
    $value = '';
    foreach($this->value as $v) {
      $value .= '%'.CRM_Core_DAO::VALUE_SEPARATOR.$v.CRM_Core_DAO::VALUE_SEPARATOR.'%';
    }
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $value, $operator);
  }

}