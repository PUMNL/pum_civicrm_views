<?php
/**
 * @author Jaap Jansma (CiviCooP) <jaap.jansma@civicoop.org>
 * @license http://www.gnu.org/licenses/agpl-3.0.html
 */

class pum_civicrm_handler_sort_case_status extends views_handler_sort {

  /**
   * Provide a list of options for the default sort form.
   * Should be overridden by classes that don't override sort_form
   */
  function sort_options() {
    return array(
      'ASC' => t('Sort ascending'),
      'DESC' => t('Sort descending'),
      'weight' => t('Sort on weight'),
    );
  }

  /**
   * Display whether or not the sort order is ascending or descending
   */
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('Exposed');
    }
    switch ($this->options['order']) {
      case 'weight':
        return t('Weight');
        break;
      case 'ASC':
      case 'asc':
      default:
        return t('asc');
        break;
      case 'DESC';
      case 'desc';
        return t('desc');
        break;
    }
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    if ($this->options['order'] == 'weight') {
      $this->ensure_my_table();
      $option_value_alias = $this->table_alias.'_case_status_option_value';
      $option_group_alias = $this->table_alias.'_case_status_option_group';


      $option_value_join = new views_join();
      $option_value_join->table = 'civicrm_option_value';
      $option_value_join->field = 'value';
      $option_value_join->left_table = 'civicrm_case';
      $option_value_join->left_field = 'status_id';
      $option_value_join->type = 'LEFT';

      $option_group_join = new views_join();
      $option_group_join->table = 'civicrm_option_group';
      $option_group_join->field = 'id';
      $option_group_join->left_table = 'civicrm_option_value';
      $option_group_join->left_field = 'option_group_id';
      $option_group_join->type = 'LEFT';
      $option_group_join->extra = array(array('field' => 'name', 'value' => 'case_status'));

      $this->query->add_relationship($option_value_alias, $option_value_join, 'civicrm_option_value', $this->table_alias);
      $this->query->add_relationship($option_group_alias, $option_group_join, 'civicrm_option_group', $option_value_alias);
      $this->query->add_orderby($this->table_alias.'_case_status_option_value', 'weight', 'ASC');
    } else {
      parent::query();
    }
    /*$this->ensure_my_table();
    // Add the field.
    $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order']);*/
  }

}