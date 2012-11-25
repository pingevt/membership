<?php

/**
 * @file
 * Cevents type editing UI.
 */

/**
 * UI controller.
 */
class CeventsTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage cevents entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the cevents type editing form.
 */
function cevents_type_form($form, &$form_state, $cevents_type, $op = 'edit') {

  if ($op == 'clone') {
    $cevents_type->label .= ' (cloned)';
    $cevents_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $cevents_type->label,
    '#description' => t('The human-readable name of this cevents type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($cevents_type->type) ? $cevents_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $cevents_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'cevents_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this cevents type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save cevents type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$cevents_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete cevents type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('cevents_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function cevents_type_form_submit(&$form, &$form_state) {
  $cevents_type = entity_ui_form_submit_build_entity($form, $form_state);
  $cevents_type->save();
  $form_state['redirect'] = 'admin/structure/cevents_types';
}

/**
 * Form API submit callback for the delete button.
 */
function cevents_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/cevents_types/manage/' . $form_state['cevents_type']->type . '/delete';
}
