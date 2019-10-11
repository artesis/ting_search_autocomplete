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

 * @return array
 *   Suggestions collection.
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
      $access_token = variable_get('ting_search_autocomplete_access_token', FALSE);
      if (!$access_token) {
        return array();
      }

      $suggestion_query = array(
        'access_token' => $access_token,
        'q' => '"' . $query . '"',
      );

      $options = array(
        'headers' => array(
          'Content-Type' => 'application/json',
        ),
      );

      $suggestion_url = url($service_url . 'suggest', array('absolute' => TRUE, 'query' => $suggestion_query));

      $result = drupal_http_request($suggestion_url, $options);

      if ($result->code == '200') {
        $suggestions = ting_search_autocomplete_prepare_json_response($result->data);

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

  return $suggestions;
}

/**
 * Prepare data for frontend from raw JSON.
 *
 * @param string $content
 *  JSON formatted content.
 *
 * @return array prepared data.
 */
function ting_search_autocomplete_prepare_json_response($content) {
  $suggestions = array();
  $content = json_decode($content);
  if (isset($content->data) && is_array($content->data)) {
    foreach ($content->data as $object) {
      $suggestions[$object->term] = $object->term;
    }
  }

  return $suggestions;
}
