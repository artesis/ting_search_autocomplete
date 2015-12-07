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

      $q = array(
        'query' => $query,
        'index' => $index,
        'filterQuery' => $filter,
        'sort' => $sort,
        'maxSuggestions' => $count,
        'agency' => variable_get('ting_agency', ''),
        'profile' => variable_get('ting_search_profile', ''),
      );
      $url = url($service_url . 'rest/terms', array('absolute' => TRUE, 'query' => $q));
      $result = drupal_http_request($url);

      if ($result->code == '200') {
        $raw_suggestions = $result->data;
        $parsed_suggestions = json_decode($raw_suggestions);

        if (isset($parsed_suggestions->suggestions) && is_array($parsed_suggestions->suggestions)) {
          foreach ($parsed_suggestions->suggestions as $suggestion) {
            $suggestions[] = array(
              'id' => $suggestion->suggestion,
              'name' => $suggestion->suggestion,
              'label' => $suggestion->suggestion,
            );
          }
        }

        cache_set($cid, $suggestions, 'cache_ting_search_autocomplete', CACHE_TEMPORARY);
      }
      else {
        $t = array(
          '!code' => $result->code,
          '!error' => $result->error,
          '!url' => $url,
        );
        watchdog('ting_search_autocomplete', 'Failed to fetch suggestions from !url. Code: !code. Error: !error.', $t, WATCHDOG_ERROR);
      }
    }
  }

  drupal_json_output($suggestions);
  drupal_exit(0);
}
