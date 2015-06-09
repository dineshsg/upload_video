<?php

function list_tutorials(){
        $sfoss = $_REQUEST['foss'];
		$slang = $_REQUEST['lang'];
	    if($_REQUEST['foss'] && $_REQUEST['lang']) {
			
			$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
	    }elseif($_REQUEST['foss']){
	        $query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted'  and td.foss_category='".$_REQUEST['foss']."'";
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted'  and td.foss_category='".$_REQUEST['foss']."'";
	    }elseif($_REQUEST['lang']){
	        $query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and tr.language='".$_REQUEST['lang']."'";
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and tr.language='".$_REQUEST['lang']."'";
	    
		}else {
			$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted'";
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted'";
		}
	$header = array(
		array('data'=>'Sno'),
		array('data'=>'Foss', 'field'=>'td.foss_category', 'sort'=>'asc'),
		array('data'=>'Lang', 'field'=>'tr.language'),
		array('data'=>'Tutorial Name', 'field'=>'td.tutorial_name'),
		array('data'=>'Outline', 'field'=>'tr.tutorial_outline_status'),
		array('data'=>'Script', 'field'=>'tr.tutorial_script_status'),
		array('data'=>'Video', 'field'=>'tr.tutorial_video_status'),
		array('data'=>'Slide', 'field'=>'tcc.tutorial_slide_status'),
		array('data'=>'Code', 'field'=>'tcc.tutorial_code_status'),
		array('data'=>'Assign', 'field'=>'tcc.tutorial_assignment_status'),
		array('data'=>'Action')
	);
	$query .= tablesort_sql($header);
	$result = pager_query($query, 30, 0, $sql_count);

	$row = array();
	$sno = 1;
	while ($item = db_fetch_object($result)) {
			$link_str = "<a href='".base_path()."stupload/play_video?tr=".$item->trid."'>View</a>";
		$row[] = array($sno++, $item -> foss_category, $item -> language, $item -> tutorial_name, component_status_symbols($item->tutorial_outline_status), component_status_symbols($item->tutorial_script_status),component_status_symbols($item->tutorial_video_status), component_status_symbols($item->tutorial_slide_status), component_status_symbols($item->tutorial_code_status),component_status_symbols($item->tutorial_assignment_status), $link_str);
		
	}
    $output .= drupal_get_form('list_tutorials_form', $sfoss, $slang);
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}

	return $output;
}

function list_tutorials_form($form_state, $sfoss, $slang){
	global $user;

	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/stupload-view.css');
	
	//fetching list of foss category details
	$query = "select distinct foss_category from tutorial_details ORDER BY foss_category ASC";
	$result = db_query($query);
	$foss_list = array();
	$foss_list[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss_list[$row->foss_category] = $row->foss_category;
	}
	
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['foss'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#default_value' => $sfoss,
		'#options' => $foss_list,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div style="width: 70%; margin: auto;"><table class="foss_search" align="Center" style="border-collapse: inherit; width: 70%;" cellpadding="7"><tr><td>',
		'#suffix' => '</td>'
	);
	
	$query = "select name from tutorial_languages ORDER BY name ASC";
	$result = db_query($query);
	$lang = array();
	$lang[''] = "Select Language";
	$lang['English'] = "English";
	while($row = db_fetch_object($result)){
		$lang[$row->name] = $row->name;
	}
	$form['flag'] = array(
		'#type' => 'hidden',
		'#value' => $flag
	);
	$form['lang'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $lang,
		'#default_value' => $slang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<td>',
		'#suffix' => '</td>'
	);
	$form['save'] = array(
		'#type' => 'submit',
		'#value' => 'Search',
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table></div>',
		'#attributes' => array('class' => 'form-select locate_tutorial')
	);
	
	return $form;
}

function list_tutorials_form_validate($form, &$form_state){
    return true;
}

function list_tutorials_form_submit($form, &$form_state){

    $redirect = 'stupload/view_tutorials_list';
	$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang'] );
	drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);

}



