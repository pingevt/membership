<?php

/**
 * @file
 * Generates a first name + last name format.
 */

$plugin = array(
  'title' => t('Type'),
  'format callback' => 'addressfield_format_type_generate',
  'type' => 'type',
  'weight' => -10,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_type_generate(&$format, $address, $context = array()) {

print '<pre>';
print_r($format);
print '</pre>';
  
  if($context['mode'] == 'render') {
  
  }
  else {
    
    $format['type_block'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('type')),
      '#weight' => -100,
    );
    $format['type_block']['type_name'] = array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#options' => array(
        0 => 'Home',
        1 => 'Work',
        2 => 'Other',
      ),
      '#attributes' => array('class' => array('address-type')),
    );
  }
}