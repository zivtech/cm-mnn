(function ($) {
  Drupal.behaviors.mnnQnapShowMatcher = {
    attach: function (context, settings) {
      ////////////////////////////////////////////////////////////////////////
      //ON READY FUNCTION
      $(document).ready(function() {
	console.log('hi mom');
	$('#mnn_cm_xml_wrapper').hide();
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR VOLUNTEER TYPE FIELD
      $("select[id$='edit-mnn-cm-airing']", context).
	change(function() {
	  console.log('hi dad');
	  mnnQnapShowMatcherSelects();
	});

      ////////////////////////////////////////////////////////////////////////
      //FUNCTION WILL HANDLE INTERACTIONS BETWEEN POSITION AND OPPORTUNITY TYPE
      function mnnQnapShowMatcherSelects() {
	var $xml_file_select = $('#edit-mnn-cm-xml-files');
	var airing =  $('#edit-mnn-cm-airing').val();

	console.log('airing: ' + airing);

	if (airing > 0) {
	  var cm_url = '/show/xml/?airing_id=' + airing;
	  console.log(cm_url);
	  //REMOVE CURRENT OPTIONS, REPLACE WITH OPTIONS FROM AJAX CALL
	  $xml_file_select.empty();
	  $.getJSON(cm_url, function(data){
	    $.each(data, function(i,item){
	      console.log(item.id);
	      console.log(item.label);
	      if (item.id==666) {
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

