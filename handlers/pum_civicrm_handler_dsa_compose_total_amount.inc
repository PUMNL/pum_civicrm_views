<?php

class pum_civicrm_handler_dsa_compose_total_amount extends civicrm_handler_field_money {

  function query() {
    $this->ensure_my_table();
    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $field = '(`'.$this->table_alias.'`.`amount_dsa` + `'.$this->table_alias.'`.`amount_briefing` + `'.$this->table_alias.'`.`amount_airport` + `'.$this->table_alias.'`.`amount_transfer` + `'.$this->table_alias.'`.`amount_hotel` + `'.$this->table_alias.'`.`amount_visa` + `'.$this->table_alias.'`.`amount_medical` + `'.$this->table_alias.'`.`amount_other` + `'.$this->table_alias.'`.`amount_advance`)';
    $this->field_alias = $this->query->add_field(null, $field, 'total_amount');

    $this->add_additional_fields();
  }

}