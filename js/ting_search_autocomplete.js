(function($) {
 Drupal.behaviors.tingSearchAutocomplete = {
    attach: function(context) {
      /**
       * Function enabling the advanced search feature.
       **/
      function tingEnableAdvancedSearch() {

        // Show advanced search.
        $('.fieldset-legend').show();
        $('.block-search-form form .extendsearch-advanced').addClass('enabled');

        // Enable autocomplete.
        $('.block-search-form form input[name="search_block_form"]').autocomplete({
          minLength: 3,
          source: function(request, response) {
            jsonReq = $.getJSON(Drupal.settings.basePath + 'ting/autocomplete', {
              query: request.term
            }, response);
          },
          search: function(event, ui) {

            // When a search is beginning, show the spinner.
            $('.block-search-form form input[name="search_block_form"]').addClass('spinner');
            $('.block-search-form form input[name="search_block_form"]').parent().addClass('spinner-wrapper');
          },
          open: function(event, ui) {

            // When a search is done, use this, to hide the spinner.
            $('.block-search-form form input[name="search_block_form"]').removeClass('spinner');
            $('.block-search-form form input[name="search_block_form"]').parent().removeClass('spinner-wrapper');
          },
          select: function(event, ui) {

            // Add the chosen value to the searchbox and submit.
            if (ui.item) {
              $('.block-search-form form input[name="search_block_form"]').val(ui.item.value);
              $('#search-block-form').submit();
            }
          }
        });        
      }

      /**
       * Function disabling the advanced search feature.
       **/    
      function tingDisableAdvancedSearch() {
        // Hide advanced search.
        $('.block-search-form form .extendsearch-advanced').removeClass('enabled');
        if ($('.block-search-form form input[name="search_block_form"]').hasClass('spinner')) {
          $('.block-search-form form input[name="search_block_form"]').removeClass('spinner');
          $('.block-search-form form input[name="search_block_form"]').parent().removeClass('spinner-wrapper');
        }

        // Disable autocomplete.
        $('.block-search-form form input[name="search_block_form"]').autocomplete({

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
      function tingMoveAdvancedSearchValues() {
        var fieldValue = $('.block-search-form form input[name="search_block_form"]').val();
        $('.block-search-form .extendsearch-advanced input').each(function() {
          if ($(this).val().length > 0) {
            fieldValue += ' ' + $(this).val();
            $(this).val('');
          }
        });
        $('.block-search-form form .ui-autocomplete-input').val(fieldValue);
      }

      // Register event for clicking MATERIAL.
      $('input#edit-search-provider-ting.form-radio').live('click', function() {
        tingEnableAdvancedSearch();
      });

      // Register event for clicking WEBSITE and E-RESOURCES.
      $('input#edit-search-provider-node.form-radio, input#edit-search-provider-meta.form-radio').live('click', function() {
        tingDisableAdvancedSearch();
        tingMoveAdvancedSearchValues();
      });
 
      tingEnableAdvancedSearch();
    }
  };
} (jQuery));