<?php

////////////////////////////////////////////////////////////////////////////
//LETTER FOR A SEASON OF ALL ACTIVE SERIES PROJECTS
function mnn_programming_exports_letters_pdf() {
  $project_data =  $_SESSION['mnn_active_series_project_data']; 
  $airing_data = $_SESSION['mnn_project_airing_data']; 

  $letters = array();
  foreach($airing_data as $airing) {
    $letters[$airing['project_id']] = $letters[$airing['project_id']] ? 
      $letters[$airing['project_id']] :array();
    $letters[$airing['project_id']][] = $airing;
  }

  mnn_programming_exports_letter_pdfbuild($project_data, $letters);
}

////////////////////////////////////////////////////////////////////////////
//LETTER FOR A SINGLE SERIES PROJECT
function mnn_programming_exports_project_letter_pdf ($project) {
  $var = 'mnn_pnid_'.$project->nid."_project_data";
  $project_data = $_SESSION[$var];
  
  $var = 'mnn_pnid_'.$project->nid."_airing_data";
  $airing_data = $_SESSION[$var];

  foreach($airing_data as $airing) {
    $letters[$project->nid] = $letters[$project->nid] ? 
      $letters[$project->nid] : array();
    $letters[$project->nid][] = $airing;
  }

  mnn_programming_exports_letter_pdfbuild($project_data, $letters);
}


function mnn_programming_exports_letter_pdfbuild($project_data, $letters) {
  set_time_limit(0);

  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);
  
  $ldate = date('M j, Y');

  $pdf = new TCPDF('Portrait', PDF_UNIT, PDF_PAGE_FORMAT, true); 
  $pdf->SetMargins(8,8,8, true);
  $pdf->setPrintHeader(FALSE);
  $pdf->AddPage();
  $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 8);
  
  $is_first = TRUE;
  foreach ($letters as $project_id=>$airings) {

    if (!$is_first) {
      $pdf->AddPage();
    }
    else {
      $is_first = FALSE;
    }
    $project_array = $project_data[$project_id];
    $quarter_data = $project_array['quarter'];

    $dates = explode( '--', $quarter_data);
    $quarter = mnn_programming_exports_get_quarter_name(strtotime($dates[0]));
    $quarter_start = date('F d, Y', strtotime($dates[0]));
    $quarter_end = date('F d, Y', strtotime($dates[1]));

    if ($project_array['format_tid'] ==
	MNN_PROGRAMMING_EXPORTS_LIVE_FORMAT_TID){
      $letter_text = "Below is your confirmed schedule for the $quarter Quarter, which runs from $quarter_start through $quarter_end. <b>Please note that your Project ID is an important identifier that MNN uses to track and program your shows.</b>  You may be asked for this number when submitting requests or communicating with MNN.";

      $letter_text .= "<br/><br/><b>Since you have a live show, MNN has made your studio reservations in advance for the entire quarter.</b>";
      
      $letter_text .= "<br/><br/><b>In the unlikely event that you cannot go live as scheduled, you must upload your program digitally to MNN in order for your program to be cablecast on our channels.&nbsp;&nbsp;</b>
      Please follow the media submission guidelines outlined in this packet of materials.";

      $letter_text .= "<br/><br/>You can upload files from your own computer or our upload stations in the 59th St. Facility Lobby. Certified, non-linear producers can upload files from the 59th St. Studios second floor edit stations.";

    }
 /******************   NEW PRODUCER LETTERS ARE BEING DONE IN ANOTHER MANNER
    else if ($project_array['season_number'] <= 1) {
      $letter_text = "Welcome to Manhattan Neighborhood Network! Below is your confirmed schedule for the $quarter Quarter, which runs from $quarter_start through $quarter_end. <b>Please note that your Project ID is an important identifier that MNN uses to track and program your shows.</b>        You may be asked for this number when submitting requests or communicating with MNN.";

      $letter_text .= "<br/><br/>Please note that this is the last quarter you will receive labels. As of the Winter 2015 Quarter, MNN will only accept digital submissions. You can upload files from your own computer or our upload stations in the 59th St. Facility Lobby. Certified, non-linear producers can upload files from the 59th St. Studios second floor edit stations. ";

    }
 **********************************************************/
    else { 
      $letter_text = "Below is your confirmed schedule for the $quarter Quarter, which runs from $quarter_start through $quarter_end.<b> Please note that your Project ID is an important identifier that MNN uses to track and program your shows.&nbsp;</b>  You may be asked for this number when submitting requests or communicating with MNN.";
      
      $letter_text .= "<br/><br/><b>You must upload your program digitally to MNN in order for your program to be cablecast on our channels.&nbsp;&nbsp;</b> 
      You can upload files from your own computer or our upload stations in the 59th St. Facility Lobby. Certified, non-linear producers can upload files from the 59th St. Studios second floor edit stations.";
      
    }  
    $thank_out_text = "Thank you for your involvement with MNN, and best wishes for a successful quarter!";
    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 8);
    $pdf->Write(4, date('M j, Y'));
    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 16);
    $pdf->Write(4, "P-".$project_id, '', 0, 'R');

    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 12);
    $pdf->Write(4, "MNN $quarter Quarter Confirmation", '', 0, 'C');

    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 11);
    //$pdf->Write(4, $letter_text);
    $pdf->writeHTML($letter_text, false, 0, false, 0);

    $pdf->Ln();
    $pdf->Ln();

    $pdf->Write(4, $thank_out_text);
    $pdf->Ln();
    $pdf->Ln();
    	 
    $pdf->Write(4, "Name: ".$project_array['display_name'] );
    $pdf->Ln();
    $pdf->Write(4, "Project ID: ".$project_id);
    $pdf->Ln();
    $pdf->Write(4, "Show Name: ".$project_array['title']);
    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 12);
    $pdf->Write(4, "Confirmed $quarter Quarter Schedule");
    $pdf->Ln();

    $pdf->SetFont("dejavusans", "", 9);
    //FIXME: how can this be generalized? 

    list($paras, $airings_rows, $airings_header) = 
      mnn_programming_exports_letter_template($project_array, $airings);

    mnn_programming_exports_write_paragraphs($pdf, $paras);

    if (count($airings) > 35 ) {
      $pdf->Write(4, "  (See next page.)");
      mnn_programming_exports_write_address($pdf);
      $pdf->AddPage();
      $pdf->SetFont("dejavusans", "", 8);
      $pdf->Write(4, $ldate);
      $pdf->SetFont("dejavusans", "B", 16);
      $pdf->Write(4, "P-".$project_id, '', 0, 'R');
      $pdf->SetFont("dejavusans", "B", 8);
      $pdf->Ln();
      $pdf->Ln();
    }

    $pdf->SetFont("dejavusans", "", 10);
    $pdf->Ln();
    $airings_table = theme_table(array('header'=>$airings_header, 
				       'rows'=>$airings_rows,
				       'attributes'=>array()));

    $pdf->writeHTML($airings_table, false, 0, false, 0);
    mnn_programming_exports_write_address($pdf);
  } 

  $pdf->lastPage();
  $pdf->Output("mnn_letter_batch.pdf", "I", "I");
}

function mnn_programming_exports_write_next_element(&$pdf, $text, &$x, &$y, $x_adder, $y_adder, $style) {
  $pdf->SetFont("dejavusans", $style, 11);
  $y += $y_adder;
  $x += $x_adder;
  $pdf->SetXY($x,$y);
  $pdf->write(2,$text);
}
 
/**
 *  these functions have mnn in the name to remind us how very mnn-specific they are
 */
function mnn_programming_exports_letter_template($project_array, $airings) {
  $quarter_data = $project_array['quarter'];

  $dates = explode( '--', $quarter_data);
  $quarter = mnn_programming_exports_get_quarter_name(strtotime($dates[0]));
  $quarter_start = date('F d, Y', strtotime($dates[0]));
  $quarter_end = date('F d, Y', strtotime($dates[1]));

  $paras = array(
		 );
  
  
  $airing_rows = array();

  foreach ( $airings as $a ) {
    $airing_rows[] = array ( 'data' => 
			     array (  // list of cells
				    array ( 'data' => $a['orig_channel']
					    ),
				    array ( 'data' => $a['airdate']
					    ),
				    array ( 'data' => $a['airtime']
					    ),
				    //array ( 'data' => $a['episode_number'] ),
				      ),
			     );
  }
  $airings_header = array( 
      array( 'data' => "<strong>Channel</strong>",
	     ),
      array( 'data' => "<b>Air Date</b>",
	     ),
      array( 'data' => "<strong><b>Air Time</b></strong>",
	     ),
//      array( 'data' => "<strong><b>Episode Number</b></strong>",),
			   );

  return array( $paras, $airing_rows, $airings_header );
}

function mnn_programming_exports_write_paragraphs(&$pdf, $paras) {
  foreach ( $paras as $p ) {
    $pdf->writeHTML($p, false, 0, false, 0);
    $pdf->Ln();
    $pdf->Ln();
  }
}

function mnn_programming_exports_write_address(&$pdf) {
  $pdf->SetFont("dejavusans", "B", 6);
  $alines = array(
		  "Manhattan Neighborhood Network",
		  "537 West 59th Street New York, NY 10019",
		  "ph: 212-757-2670 fx: 212-757-1603 http://www.mnn.org",
		  );




  if ( $pdf->GetY() < 256 ) {
    $pdf->SetY(264);
  }
  else { 
    $pdf->Ln();
  }
  foreach ( $alines as $l ) {
    $pdf->Write(4, $l, '', 0, 'C');
    $pdf->Ln();
  }
  $pdf->SetFont("dejavusans", "", 8);
}


function mnn_programming_exports_get_quarter_name($unixtime) {
  //FIXME: horribly hardcoded!
  $r = array(
	     3 => "Spring",
	     4 => "Spring",
	     5 => "Spring",
	     6 => "Summer",
	     7 => "Summer",
	     8 => "Summer",
	     9 => "Fall",
	     10 => "Fall",
	     11 => "Fall",
	     12 => "Winter",
	     1 => "Winter",
	     2 => "Winter",
	     );
  $month = date('n', $unixtime);
  $year = date('Y', $unixtime);
  if ( $month == 12 ) {
    $year++;
  }
  return $r[$month] . ' ' . $year;
}
