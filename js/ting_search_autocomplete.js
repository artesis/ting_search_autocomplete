(function($) {
  Drupal.behaviors.tingSearchAutocomplete = {
    attach: function(context) {
      /**
       * Function enabling the advanced search feature.
       **/
      function tingEnableAdvancedSearch($search_form) {

        // Show advanced search.
        $('.fieldset-legend').show();
        $('.block-search-form form .extendsearch-advanced').addClass('enabled');

        // Enable autocomplete.
        $search_form.autocomplete({
          minLength: 3,
          source: function(request, response) {
            jsonReq = $.getJSON(Drupal.settings.basePath + 'ting/autocomplete/' + request.term, {}, response);
          },
          search: function(event, ui) {
            // When a search is beginning, show the spinner.
            $search_form.addClass('spinner');
            $search_form.parent().addClass('spinner-wrapper');
          },
          open: function(event, ui) {
            // When a search is done, use this, to hide the spinner.
            $search_form.removeClass('spinner');
            $search_form.parent().removeClass('spinner-wrapper');
          },
          select: function(event, ui) {
            // Add the chosen value to the searchbox and submit.
            if (ui.item) {
              $search_form.val(ui.item.value);
              $('#search-block-form').submit();
            }
          }
        });
      }

      /**
       * Function disabling the advanced search feature.
       **/
      function tingDisableAdvancedSearch($search_form) {
        // Hide advanced search.
        $('.block-search-form form .extendsearch-advanced').removeClass('enabled');
        if ($search_form.hasClass('spinner')) {
          $search_form.removeClass('spinner');
          $search_form.parent().removeClass('spinner-wrapper');
        }

        // Disable autocomplete.
        $search_form.autocomplete({

          // Overwrite source function.
          source: [],

          // Overwrite search function.
          search: function(event, ui) {
          },

          // Overwrite open function.
          open: function(event, ui) {
          },

          // Overwrite select function.
          select: function(event, ui) {
          }
        });
      }

      /**
       * Function moves advanced search values to default search field.
       **/
      function tingMoveAdvancedSearchValues($search_form) {
        var fieldValue = $search_form.val();
        $('.block-search-form .extendsearch-advanced input').each(function() {
          if ($(this).val().length > 0) {
            fieldValue += ' ' + $(this).val();
            $(this).val('');
          }
        });
        $('.block-search-form form .ui-autocomplete-input').val(fieldValue);
      }

      let $search_form = $('.block-search-form form input[name="search_block_form"]', context);
      // Check if default selected search provider is ting.
      if ($('input[name="search_provider"]:checked').val() === 'ting') {
        tingEnableAdvancedSearch($search_form);
      }
      else {
        tingDisableAdvancedSearch($search_form);
        tingMoveAdvancedSearchValues($search_form);
      }

      // Register event for clicking search provider radio.
      $('input#edit-search-provider-ting.form-radio').on('click', function() {
        tingEnableAdvancedSearch($search_form);
      });

      // Register event for clicking WEBSITE and E-RESOURCES.
      $('input#edit-search-provider-node.form-radio, input#edit-search-provider-meta.form-radio').on('click', function() {
        tingDisableAdvancedSearch($search_form);
        tingMoveAdvancedSearchValues($search_form);
      });
    }
  };
} (jQuery));
