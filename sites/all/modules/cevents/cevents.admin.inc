<?php

/**
 * @file
 * Cevents editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class CeventsUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Ceventss',
      'description' => 'Add edit and update ceventss.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of ceventss.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a cevents',
      'description' => 'Add a new cevents',
      'page callback'  => 'cevents_add_page',
      'access callback'  => 'cevents_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'cevents.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (cevents_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'cevents_form_wrapper',
        'page arguments' => array(cevents_create(array('type' => $type->type))),
        'access callback' => 'cevents_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'cevents.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing cevents entities
    $items[$this->path . '/cevents/' . $wildcard] = array(
      'page callback' => 'cevents_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'cevents_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'cevents.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/cevents/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/cevents/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'cevents_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'cevents_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'cevents.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing ceventss
    $items['cevents/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'cevents_page_title',
      'title arguments' => array(1),
      'page callback' => 'cevents_page_view',
      'page arguments' => array(1),
      'access callback' => 'cevents_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Cevents Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('cevents_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: create or edit a cevents.
 *
 * @param $cevents
 *   The cevents object being edited by this form.
 *
 * @see cevents_edit_form()
 */
function cevents_form_wrapper($cevents) {
  // Add the breadcrumb for the form's location.
  cevents_set_breadcrumb();
  return drupal_get_form('cevents_edit_form', $cevents);
}


/**
 * Form callback wrapper: delete a cevents.
 *
 * @param $cevents
 *   The cevents object being edited by this form.
 *
 * @see cevents_edit_form()
 */
function cevents_delete_form_wrapper($cevents) {
  // Add the breadcrumb for the form's location.
  //cevents_set_breadcrumb();
  return drupal_get_form('cevents_delete_form', $cevents);
}


/**
 * Form callback: create or edit a cevents.
 *
 * @param $cevents
 *   The cevents object to edit or for a create form an empty cevents object
 *     with only a cevents type defined.
 */
function cevents_edit_form($form, &$form_state, Cevents $cevents) {
  // Add the default field elements.
  $form['date'] = array(
    '#type' => 'date',
    '#title' => t('Date of the Cevents'),
    '#default_value' => isset($cevents->date) ? $cevents->date : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['data']['#tree'] = TRUE;

  // Add the field related form elements.
  $form_state['cevents'] = $cevents;
  field_attach_form('cevents', $cevents, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save cevents'),
    '#submit' => $submit + array('cevents_edit_form_submit'),
  );

  if (!empty($cevents->date)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete cevents'),
      '#suffix' => l(t('Cancel'), 'admin/content/ceventss'),
      '#submit' => $submit + array('cevents_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'cevents_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the cevents form
 */
function cevents_edit_form_validate(&$form, &$form_state) {
  $cevents = $form_state['cevents'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('cevents', $cevents, $form, $form_state);
}


/**
 * Form API submit callback for the cevents form.
 *
 * @todo remove hard-coded link
 */
function cevents_edit_form_submit(&$form, &$form_state) {

  $cevents = entity_ui_controller('cevents')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the cevents and go back to the list of ceventss

  // Add in created and changed times.
  if ($cevents->is_new = isset($cevents->is_new) ? $cevents->is_new : 0){
    $cevents->created = time();
  }

  if(is_array($cevents->date)) {
    $cevents->date = mktime(0, 0, 0, $cevents->date['month'], $cevents->date['day'], $cevents->date['year']);
  }

  $cevents->changed = time();

  $cevents->save();
  $form_state['redirect'] = 'admin/content/ceventss';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function cevents_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/ceventss/cevents/' . $form_state['cevents']->cevents_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a cevents.
 *
 * @param $cevents
 *   The cevents to delete
 *
 * @see confirm_form()
 */
function cevents_delete_form($form, &$form_state, $cevents) {
  $form_state['cevents'] = $cevents;

  $form['#submit'][] = 'cevents_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete cevents %date?', array('%date' => $cevents->date)),
    'admin/content/ceventss/cevents',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for cevents_delete_form
 */
function cevents_delete_form_submit($form, &$form_state) {
  $cevents = $form_state['cevents'];

  cevents_delete($cevents);

  drupal_set_message(t('The cevents %date has been deleted.', array('%date' => $cevents->date)));
  watchdog('cevents', 'Deleted cevents %date.', array('%date' => $cevents->date));

  $form_state['redirect'] = 'admin/content/ceventss';
}



/**
 * Page to add Cevents Entities.
 *
 * @todo Pass this through a proper theme function
 */
function cevents_add_page() {
  $controller = entity_ui_controller('cevents');
  return $controller->addPage();
}


/**
 * Displays the list of available cevents types for cevents creation.
 *
 * @ingroup themeable
 */
function theme_cevents_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="cevents-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer cevents types')) {
      $output = '<p>' . t('Cevents Entities cannot be added because you have not created any cevents types yet. Go to the <a href="@create-cevents-type">cevents type creation page</a> to add a new cevents type.', array('@create-cevents-type' => url('admin/structure/cevents_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No cevents types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative cevents pages.
 */
function cevents_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Ceventss'), 'admin/content/ceventss'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



