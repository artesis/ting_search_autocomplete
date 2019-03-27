<?php
/**
 * @file
 * Ting search autocomplete admin related.
 */

/**
 * Form builder for the autosuggestions configuration form.
 */
function ting_search_autocomplete_admin_form($form, $form_state) {
  $form['ting_search_autocomplete_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Service URL'),
    '#default_value' => variable_get('ting_search_autocomplete_url', ''),
    '#description' => t('e.g.: https://openplatform.dbc.dk/v3/'),
    '#required' => TRUE,
  );

  $form['ting_search_autocomplete_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('ting_search_autocomplete_client_id', ''),
    '#required' => TRUE,
  );

  $form['ting_search_autocomplete_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret key'),
    '#default_value' => variable_get('ting_search_autocomplete_client_secret', ''),
    '#required' => TRUE,
  );

  $form['#submit'][] = 'ting_search_autocomplete_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Custom validation callback for autocomplete suggestions settings form.
 *
 * @see ting_search_autocomplete_admin_form()
 */
function ting_search_autocomplete_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!valid_url($values['ting_search_autocomplete_url'], TRUE)) {
    form_set_error('ting_search_autocomplete_url', t('Enter a valid service URL.'));
  }
}

/**
 * Generate access token.
 */
function ting_search_autocomplete_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  ting_search_autocomplete_generate_token($values['ting_search_autocomplete_client_id'], $values['ting_search_autocomplete_client_secret']);
}
