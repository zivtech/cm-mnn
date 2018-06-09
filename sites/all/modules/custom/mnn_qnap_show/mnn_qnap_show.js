(function ($) {
  Drupal.behaviors.mnnQnapShowMatcher = {
    attach: function (context, settings) {
      ////////////////////////////////////////////////////////////////////////
      //ON READY FUNCTION
      $(document).ready(function() {
	var airing =  $('#edit-mnn-cm-airing').val();

	if (airing > 0) {
	  mnnQnapShowMatcherSelects();
	}
	else {
	  $('#mnn_cm_xml_wrapper').hide();
	}
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR VOLUNTEER TYPE FIELD
      $("select[id$='edit-mnn-cm-airing']", context).
	change(function() {
	  mnnQnapShowMatcherSelects();
	});

      ////////////////////////////////////////////////////////////////////////
      //FUNCTION WILL HANDLE INTERACTIONS BETWEEN AIRDATE AND XML FILENAME
      function mnnQnapShowMatcherSelects() {
	var $xml_file_select = $('#edit-mnn-cm-xml-files');
	var airing =  $('#edit-mnn-cm-airing').val();

	if (airing > 0) {
	  var cm_url = '/show/xml/?airing_id=' + airing;
	  var exact_match = false;
	  console.log(cm_url);
	  //REMOVE CURRENT OPTIONS, REPLACE WITH OPTIONS FROM AJAX CALL
	  $xml_file_select.empty();
	  $.getJSON(cm_url, function(data){
	    $.each(data, function(i,item){
	      if (item.id == 'exact') {
		exact_match = true;
		$xml_file_select.append($("<option></option>")
					.attr("value", item.id)
					.text(item.label));
		
	      }
	      else if (Boolean(exact_match)) {
		exact_match = false;
		$xml_file_select.append($("<option></option>")
				   .attr("value", item.id)
				   .attr('selected', true)
				   .text(item.label));
	      }
	      else {
		$xml_file_select.append($("<option></option>")
				   .attr("value", item.id)
				   .text(item.label));
	      }
	      
	    });
	  });
	  $('#mnn_cm_xml_wrapper').show();
	}
      }
    }};
}) (jQuery);

