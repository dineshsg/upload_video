<?php

function reviewed_list(){
	global $user;
	
	$query = "";
	$sql_count = "";
	if(in_array('domain_user', $user->roles) || in_array('admin_review_user', $user->roles)){
		$query = "select distinct tl.tutorial_resources_id, tr.id as trid, tr.language, tr.tutorial_status, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_update_log tl, tutorial_resources tr, tutorial_details td where tl.updated_content='review' and tl.updated_by='".$user->name."' and tr.id=tl.tutorial_resources_id and tr.tutorial_detail_id=td.id";
		$sql_count = "select count(distinct tl.tutorial_resources_id) from tutorial_update_log tl, tutorial_resources tr, tutorial_details td where tl.updated_content='review' and tl.updated_by='".$user->name."' and tr.id=tl.tutorial_resources_id and tr.tutorial_detail_id=td.id";
	}else{
		return "<h2>Permission Denied</h2>";
	}
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$output = '';
	$sno = 1;
	$header = array('Sno', 'FOSS', 'Level', 'Tutorial Name', 'Language', 'Status');
	$tutorial_status = "";
	while ($item = db_fetch_object($result)) {
		if($item->tutorial_status == 'need_improvement')
			$tutorial_status = "<div class='code_red' style='text-align : left; color : red; font-size : 12px;'>Need Improvement</div>";
		elseif($item->tutorial_status == 'accepted' || $item->tutorial_status == 'under_review')
			$tutorial_status = "<div class='code_green' style='text-align : left; color : green;	font-size : 12px;'>Accepted</div>";
		else {
			$tutorial_status = "<div class='code_red' style='text-align : left; color : red; font-size : 12px;'>".$item->tutorial_status."</div>";
		}
		
		$row[] = array($sno++, $item->foss_category, $item -> tutorial_level, $item -> tutorial_name, $item -> language, $tutorial_status);
	}
	if (count($row) == 0) {
		$output = '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}