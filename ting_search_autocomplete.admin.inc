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
    '#description' => t('e.g.: http://opensuggestion.addi.dk/b3.0_2.0/'),
    '#required' => TRUE,
  );

  $form['ting_search_autocomplete_index'] = array(
    '#type' => 'textfield',
    '#title' => t('Autocomplete index'),
    '#default_value' => variable_get('ting_search_autocomplete_index', ''),
    '#description' => t('e.g.: scanterm.mainTitle'),
    '#required' => TRUE,
  );

  $form['ting_search_autocomplete_filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Autocomplete filter'),
    '#default_value' => variable_get('ting_search_autocomplete_filter', ''),
    '#description' => t('e.g.: scanphrase.mainTitle'),
  );

  $form['ting_search_autocomplete_sort'] = array(
    '#type' => 'select',
    '#title' => t('Suggestions sort'),
    '#options' => array(
      'index' => t('Index'),
      'count' => t('Count'),
    ),
    '#default_value' => variable_get('ting_search_autocomplete_sort', 'index'),
    '#description' => t('How the suggestions should be sorted.'),
  );

  $form['ting_search_autocomplete_suggestions'] = array(
    '#type' => 'textfield',
    '#title' => t('Suggestions count'),
    '#default_value' => variable_get('ting_search_autocomplete_suggestions', TING_AUTOCOMPLETE_SUGGESTIONS_LIMIT),
    '#description' => t('Number of suggestions to fetch.'),
    '#size' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
  );

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
