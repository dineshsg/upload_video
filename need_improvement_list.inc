<?php

function need_improvement_list(){
	global $user;
	
	$query = "";
	$sql_count = "";
	if(in_array('contributor', $user->roles)){
		$query = "select tr.id as trid, tr.language, tr.tutorial_status, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and ((tr.tutorial_outline_status = 5 and tr.tutorial_outline_uid = ".$user->uid.") or (tr.tutorial_script_status=5 and tr.tutorial_script_uid=".$user->uid.") or (tutorial_video_status=5 and tutorial_video_uid=".$user->uid.") or (tcc.tutorial_slide_status=5 and tcc.tutorial_slide_uid=".$user->uid.") or (tcc.tutorial_code_status=5 and tcc.tutorial_code_uid=".$user->uid.") or (tcc.tutorial_assignment_status=5 and tcc.tutorial_assignment_uid=".$user->uid.")) and tr.tutorial_content_id=tcc.id and tr.tutorial_status!='accepted'";
		$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and ((tr.tutorial_outline_status = 5 and tr.tutorial_outline_uid = ".$user->uid.") or (tr.tutorial_script_status=5 and tr.tutorial_script_uid=".$user->uid.") or (tutorial_video_status=5 and tutorial_video_uid=".$user->uid.") or (tcc.tutorial_slide_status=5 and tcc.tutorial_slide_uid=".$user->uid.") or (tcc.tutorial_code_status=5 and tcc.tutorial_code_uid=".$user->uid.") or (tcc.tutorial_assignment_status=5 and tcc.tutorial_assignment_uid=".$user->uid.")) and tr.tutorial_content_id=tcc.id and tr.tutorial_status!='accepted'";	
	}else{
		return "<h2>Permission Denied</h2>";
	}
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$output = '';
	$sno = 1;
	$header = array('Sno', 'FOSS', 'Level', 'Tutorial Name', 'Language', 'Action');
	$tutorial_status = "";
	while ($item = db_fetch_object($result)) {
		if($item -> language == 'English'){
			$row[] = array($sno++, $item->foss_category, $item -> tutorial_level, $item -> tutorial_name, $item -> language, "<a href='".$GLOBALS['base_path']."stupload/upload_english_tutorial_index?trid=".$item -> trid."'>Update</a>");
		}else{
			$row[] = array($sno++, $item->foss_category, $item -> tutorial_level, $item -> tutorial_name, $item -> language, "<a href='".$GLOBALS['base_path']."stupload/upload_regional_language_tutorial_index?tr=".$item -> trid."'>Update</a>");
		}
	}
	if (count($row) == 0) {
		$output = '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}
