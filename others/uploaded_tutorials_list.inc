<?php

function uploaded_tutorials_list(){
	global $user;
	
	$query = "";
	$sql_count = "";
	if(in_array('contributor', $user->roles)){
		$query = "select tr.id as trid, tr.language, tr.tutorial_status, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr, tutorial_details td where tr.uid='".$user->uid."' and td.id=tr.tutorial_detail_id";
		$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td where tr.uid='".$user->uid."' and td.id=tr.tutorial_detail_id";
	}else{
		return "<h2>Permission Denied</h2>";
	}
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$output = '';
	$sno = 1;
	$header = array('Sno', 'FOSS', 'Level', 'Tutorial Name', 'Language', 'Status', '');
	$tutorial_status = "";
	while ($item = db_fetch_object($result)) {
		if($item->tutorial_status == 'need_improvement')
			$tutorial_status = "<div class='code_red' style='text-align : left; color : red; font-size : 12px;'>Need Improvement</div>";
		elseif($item->tutorial_status == 'accepted')
			$tutorial_status = "<div class='code_green' style='text-align : left; color : green;	font-size : 12px;'>Accepted</div>";
		elseif($item->tutorial_status == 'under_review')
			$tutorial_status = "<div class='code_green' style='text-align : left; color : red;	font-size : 12px;'>Under Review</div>";
		elseif($item->tutorial_status == 'reviewed')
			$tutorial_status = "<div class='code_green' style='text-align : left; color : green;	font-size : 12px;'>Reviewed</div>";
		else
			$tutorial_status = "<div class='code_red' style='text-align : left; color : red; font-size : 12px;'>".$item->tutorial_status."</div>";
		
		$row[] = array($sno++, $item->foss_category, $item -> tutorial_level, $item -> tutorial_name, $item -> language, $tutorial_status, l(t('Delete'), "stupload/delete/".$item -> trid."/".$item -> tutorial_name , array('attributes' => array('class' => 'delete-record', 'title' => 'Delete'))));
	}
	if (count($row) == 0) {
		$output = '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}
