<?php
/**
 * @file
 * XML for Cinegy
 */
?>
<?
//General template, no image
if ($node->field_cinegy_type_template[LANGUAGE_NONE][0]['tid'] == 1016) { ?>
      <event offset="+<?php print $accumulated_interstitials_duration?>0000000" device="*CG_0" cmd="SHOW" skip="0"><op1>X:\Type\okv_member_general_no_image.CinType</op1><op2>&lt;variables&gt;&lt;var name="Title.Text.Value" type="Text" value="<?php print $node->field_title[LANGUAGE_NONE][0]['value'] ?>"/&gt;&lt;var name="Body.Text.Value" type="Text" value="<?php print $node->field_okv_text[LANGUAGE_NONE][0]['value'] ?>"/&gt;&lt;var name="Logo.File.Name" type="File" value="X:\Contents\Logotypes\<?php print $node->field_logo[LANGUAGE_NONE][0]['filename'] ?>"/&gt;&lt;/variables&gt;</op2></event>
<?php } ?>
<?php
//General member/OKV template with image
if ($node->field_cinegy_type_template[LANGUAGE_NONE][0]['tid'] == 1017) { ?>
  	  <event offset="+<?php print $accumulated_interstitials_duration?>0000000" device="*CG_0" cmd="SHOW" skip="0"><op1>X:\Type\okv_member_general.CinType</op1><op2>&lt;variables&gt;&lt;var name="Title.Text.Value" type="Text" value="<?php print $node->field_title[LANGUAGE_NONE][0]['value'] ?>"/&gt;&lt;var name="Body.Text.Value" type="Text" value="<?php print $node->field_okv_text[LANGUAGE_NONE][0]['value'] ?>"/&gt;&lt;var name="Picture.File.Name" type="File" value="X:\Contents\Images\<?php print $node->field_okv_image[LANGUAGE_NONE][0]['filename'] ?>"/&gt;&lt;var name="Logo.File.Name" type="File" value="X:\Contents\Logotypes\<?php print $node->field_logo[LANGUAGE_NONE][0]['filename'] ?>"/&gt;&lt;/variables&gt;</op2></event>
<?php } ?>
<?php
//Upcoming six shows
if ($node->field_cinegy_type_template[LANGUAGE_NONE][0]['tid'] == 1058) { ?>
      <event offset="+<?php print $accumulated_interstitials_duration?>0000000" device="*CG_0" cmd="SHOW" skip="0"><op1>X:\Type\okv_upcoming_six_shows.CinType</op1><op2>&lt;variables&gt;&lt;var name="Title.Text.Value" type="Text" value="KOMMANDE"/&gt;&lt;var name="Starttime.Text.Value1" type="Text" value=""/&gt;&lt;var name="Starttime.Text.Value2" type="Text" value=""/&gt;&lt;var name="Starttime.Text.Value3" type="Text" value=""/&gt;&lt;var name="Starttime.Text.Value4" type="Text" value=""/&gt;&lt;var name="Starttime.Text.Value5" type="Text" value=""/&gt;&lt;var name="Starttime.Text.Value6" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value1" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value2" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value3" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value4" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value5" type="Text" value=""/&gt;&lt;var name="Showtitle.Text.Value6" type="Text" value=""/&gt;&lt;/variables&gt;</op2></event>
<?php } ?>
<?php
//Video with logo
if ($node->field_cinegy_type_template[LANGUAGE_NONE][0]['tid'] == 1125) { ?>
      <event offset="+<?php print $accumulated_interstitials_duration?>0000000" device="*CG_0" cmd="SHOW" skip="0"><op1>X:\Type\okv_member_video_logo.CinType</op1><op2>&lt;variables&gt;&lt;var name="Logo.File.Name" type="File" value="X:\Contents\Logotypes\<?php print $node->field_logo[LANGUAGE_NONE][0]['filename'] ?>"/&gt;&lt;var name="Video.File.Name" type="File" value="X:\Contents\Video\<?php print $node->field_okv_video[LANGUAGE_NONE][0]['value'] ?>"/&gt;&lt;/variables&gt;</op2></event>
<?php } ?>
