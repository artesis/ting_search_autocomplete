<?php
/**
 * @file
 * Autocomplete ajax handlers.
 */

/**
 * Autocomplete ajax handler.
 *
 * @param string $query
 *   Search query string.
 */
function ting_search_autocomplete($query) {
  $suggestions = array();

  if (!empty($query)) {
    $cid = md5($query);

    $cache = cache_get($cid, 'cache_ting_search_autocomplete');
    if ($cache) {
      $suggestions = $cache->data;
    }
    else {
      $service_url = variable_get('ting_search_autocomplete_url', '');
      $index = variable_get('ting_search_autocomplete_index', '');
      $filter = variable_get('ting_search_autocomplete_filter', '');
      $sort = variable_get('ting_search_autocomplete_sort', '');
      $count = variable_get('ting_search_autocomplete_suggestions', TING_AUTOCOMPLETE_SUGGESTIONS_LIMIT);

      $suggestion_query = array(
        'query' => $query,
        'index' => $index,
        'lower' => '',
        'filterQuery' => $filter,
        'sort' => $sort,
        'maxSuggestions' => $count,
        'agency' => variable_get('ting_agency', ''),
        'profile' => variable_get('ting_search_profile', ''),
      );

      $options = array(
        'headers' => array(
          'Content-Type' => 'application/json',
        ),
      );

      $suggestion_url = url($service_url . 'rest/terms', array('absolute' => TRUE, 'query' => $suggestion_query));
      $result = drupal_http_request($suggestion_url, $options);

      $spell_query = array(
          'query' => $query,
          'index' => variable_get('ting_search_spellcheck_index', ''),
          'facetIndex' => variable_get('ting_search_spellcheck_facet_index', ''),
          'filterQuery' => $filter,
          'agency' => variable_get('ting_agency', ''),
          'profile' => variable_get('ting_search_profile', ''),
          'maxSuggestions' => 3,
      );

      $spell_url = url($service_url . 'rest/facetSpell', array('absolute' => TRUE, 'query' => $spell_query));
      $spell_result = drupal_http_request($spell_url, $options);

      $spell_suggestions = array();
      if ($spell_result->code == '200') {
        $spell_suggestions = ting_search_autocomplete_prepare_json_response($spell_result->data, TRUE);
      }

      if ($result->code == '200') {
        $suggestions = ting_search_autocomplete_prepare_json_response($result->data);

        $suggestions = $suggestions + $spell_suggestions;

        cache_set($cid, $suggestions, 'cache_ting_search_autocomplete', CACHE_TEMPORARY);
      }
      else {
        $t = array(
          '!code' => $result->code,
          '!error' => $result->error,
          '!url' => $suggestion_url,
        );
        watchdog('ting_search_autocomplete', 'Failed to fetch suggestions from !url. Code: !code. Error: !error.', $t, WATCHDOG_ERROR);
      }
    }
  }

  drupal_json_output($suggestions);
  drupal_exit(0);
}

/**
 * Prepare data for frontend from raw JSON.
 *
 * @param array $data
 *  JSON data.
 * @param bool $spell
 *  Determines if JSON data for suggestion or spell response.
 *
 * @return array prepared data.
 */
function ting_search_autocomplete_prepare_json_response($data, $spell = FALSE) {
  $suggestions = array();
  $raw_suggestions = $data;
  $parsed_suggestions = json_decode($raw_suggestions);

  if (isset($parsed_suggestions->suggestions) && is_array($parsed_suggestions->suggestions)) {
    foreach ($parsed_suggestions->suggestions as $suggestion) {
      $suggestion_string = ($spell) ? $suggestion->phrase : $suggestion->suggestion;
      $suggestions[] = array(
        'id' => $suggestion_string,
        'name' => $suggestion_string,
        'label' => $suggestion_string,
      );
    }
  }

  return $suggestions;
}
