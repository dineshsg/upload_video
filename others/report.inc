<?php

function tutorials_status_report(){

	global $user;
	$query = "";
	$sql_count = "";

	if(in_array('admin_user', $user->roles)){
		$query = "select tr.id, tr.language, tr.upload_time, tr.tutorial_status, td.tutorial_name, td.foss_category from tutorial_resources tr, tutorial_details td where tr.tutorial_status!='accepted' and td.id=tr.tutorial_detail_id order by tr.upload_time ASC";
		$sql_count = "select count(tr.id) from tutorial_resources tr, tutorial_details td where tr.tutorial_status!='accepted' and td.id=tr.tutorial_detail_id";
	}else{
		return "<h2>Permission Denied</h2>";
	}
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$output = '';
	$sno = 1;
	$header = array('FOSS', 'Tutorial Name', 'Language', 'Last Updated Time', 'Status');
	while($item = db_fetch_object($result)) {
		$row[] = array(human_text($item->foss_category), human_text($item->tutorial_name), $item->language, $item->upload_time, $item->tutorial_status);
	}
	if (count($row) == 0) {
		$output = '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}
