<?php
/**
 * @file
 * XML for Cinegy
 */
?>
<?php $cinegy_file_path = variable_get('cinegy_file_path', 'X:\Contents');

if($node->field_filformat[LANGUAGE_NONE][0]['value'] <>'') {
  $extension = $node->field_filformat[LANGUAGE_NONE][0]['value'];
} else {
  $extension = '.mpg';
}
?>
 <program name="<?php print check_plain($node->title) ?>">
   <guid><?php print getGUID()?></guid>
     <block name="Block 1">

	 <guid><?php print getGUID()?></guid>
	   <item name="<?php print '(' . check_plain($node->field_program_code[LANGUAGE_NONE][0]['value']) . $node->field_filformat[LANGUAGE_NONE][0]['value'] . ') ' . check_plain($node->title) ?>" src_in="00:00:00:00" src_out="<?php print cinegy_sec2hms($node->field_okv_duration[LANGUAGE_NONE][0]['value']) ?>:00" in="00:00:00:00" out="<?php print cinegy_sec2hms($node->field_okv_duration[LANGUAGE_NONE][0]['value']) ?>:00" type="clip" flags="0x10" split_audio="0" start="<?php print $start_time ?>" date="<?php print $start_date ?>">

       <guid><?php print getGUID()?></guid>
       <FrameRate>25</FrameRate>
       <timeline duration="<?php print $accumulated_duration ?>">
	     <group type="video" width="720" height="576" framerate="25" progressive="n">
		   <track>
		     <clip start="0" stop="<?php print $node->field_okv_duration[LANGUAGE_NONE][0]['value'] ?>" mstart="0" mstop="<?php print $node->field_okv_duration[LANGUAGE_NONE][0]['value'] ?>">
			   <quality src="<?php print $cinegy_file_path ?>\<?php print $node->field_program_code[LANGUAGE_NONE][0]['value'] . $extension ?>" track="0" id="0"/>
		     </clip>
		   </track>
	     </group>
	     <group type="audio">
	       <track>
		     <clip start="0" stop="<?php print $node->field_okv_duration[LANGUAGE_NONE][0]['value'] ?>" mstart="0" mstop="<?php print $node->field_okv_duration[LANGUAGE_NONE][0]['value'] ?>">
		       <quality src="<?php print $cinegy_file_path ?>\<?php print $node->field_program_code[LANGUAGE_NONE][0]['value'] . $extension ?>" track="0" channels="0;1" id="0"/>
		     </clip>
		   </track>
	     </group>
	   </timeline>
	   <src_path><?php print $cinegy_file_path ?>\<?php print $node->field_program_code[LANGUAGE_NONE][0]['value'] . $extension?></src_path>
     <events><event offset="+0" device="*CG_0" cmd="HIDE" skip="0"/><event offset="+0" device="*CG_1" cmd="HIDE" skip="0"/><event offset="+0" device="*CG_2" cmd="HIDE" skip="0"/><event offset="+0" device="*CG_3" cmd="HIDE" skip="0"/><event offset="+0" device="*CG_4" cmd="HIDE" skip="0"/></events>
       </item>
	 </block>
   </program>

