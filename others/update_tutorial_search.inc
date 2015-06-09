<?php

function update_tutorial_search($params){
	
	$foss = '';
	$language = '';

	if($params){
		$params_data = parse_url($params);
		$data = explode('&', $params_data['query']);
		$tutorial_resourse = explode('=', $data[0]);
		$foss = $tutorial_resourse[1];
		$tutorial_resourse = explode('=', $data[1]);
		$language = $tutorial_resourse[1];
	}else{
		$foss = "";
		$language = "";
	}
	$output = drupal_get_form('update_tutorial_search_form', $foss, $language);
	return $output;
}

function update_tutorial_search_form($form_state, $foss, $language){
	global $user;
	
	if($foss != "" && $language != ""){
		$query = "select tr.id as trid, tr.language, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_script_timed, tr.tutorial_video, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tc.tutorial_slide, tc.tutorial_code, tc.tutorial_assignment from tutorial_resources tr, tutorial_details td, tutorial_common_contents tc where tr.uid=".$user->uid." and tr.tutorial_status!='accepted' and tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tc.id and tr.language='".$language."' and td.foss_category='".$foss."' order by td.order_code ASC";
		$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tc where tr.uid=".$user->uid." and tr.tutorial_status!='accepted' and tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tc.id and tr.language='".$language."' and td.foss_category='".$foss."' order by td.order_code ASC";
		$result = pager_query($query, 30, 0, $sql_count);
		$row = array();
		$output = '';
		$sno = 1;

		if($language == 'English'){
			$header = array('Sno', 'Level', 'Tutorial Name', 'Outline', 'Script', 'Timed-script', 'Video', 'Slides', 'Assignment', 'Code', '');
		}else{
			$header = array('Sno', 'Level', 'Tutorial Name', 'Outline', 'Script', 'Video', 'Slides', 'Assignment', 'Code', '');
		}
		while ($item = db_fetch_object($result)) {
			if($item->tutorial_outline == 'pending')
				$tutorial_outline = "<div class='code_red'>&#8855;</div>";
			else
				$tutorial_outline = "<div class='code_green'>&#10004</div>";
			if($item->tutorial_script == 'pending')
				$tutorial_script = "<div class='code_red'>&#8855;</div>";
			else
				$tutorial_script = "<div class='code_green'>&#10004</div>";
			if($item->tutorial_video == 'pending')
				$tutorial_video = "<div class='code_red'>&#8855;</div>";
			else
				$tutorial_video = "<div class='code_green'>&#10004</div>";
			if($item->tutorial_slide == 'pending')
				$tutorial_slide = "<div class='code_red'>&#8855;</div>";
			else
				$tutorial_slide = "<div class='code_green'>&#10004</div>";
			if($item->tutorial_assignment == 'pending')
				$tutorial_assignment = "<div class='code_red'>&#8855;</div>";
			else if($item->tutorial_assignment == 'notrequired')
				$tutorial_assignment = "<div class='code_green'>&#8722;</div>";
			else
				$tutorial_assignment = "<div class='code_green'>&#10004</div>";
			if($item->tutorial_code == 'pending')
				$tutorial_code = "<div class='code_red'>&#8855;</div>";
			else if($item->tutorial_code == 'notrequired')
				$tutorial_code = "<div class='code_green'>&#8722;</div>";
			else
				$tutorial_code = "<div class='code_green'>&#10004</div>";
			// timed secript
			if($language == 'English'){
				if($item->tutorial_script_timed == 'pending')
					$tutorial_timed_script = "<div class='code_red'>&#8855;</div>";
				else
					$tutorial_timed_script = "<div class='code_green'>&#10004</div>";

				$row[] = array($sno++, $item -> tutorial_level, $item -> tutorial_name, $tutorial_outline, $tutorial_script,  $tutorial_timed_script,  $tutorial_video, $tutorial_slide, $tutorial_assignment, $tutorial_code, "<a href='".$GLOBALS['base_path']."stupload/update_index/%3Ftr%3D".$item -> trid."'>Update</a>");
			}else{
				$row[] = array($sno++, $item -> tutorial_level, $item -> tutorial_name, $tutorial_outline, $tutorial_script, $tutorial_video, $tutorial_slide, $tutorial_assignment, $tutorial_code, l(t('Update'), "stupload/update_index/%3Ftr%3D".$item -> trid , array('attributes' => array('class' => 'update-record', 'title' => 'Update'))));
			}
		}
		if (count($row) == 0) {
			$output = '<div style="color:red;text-align:center;">List is empty.</div>';
		}else{
			$output .= "<ul><li><b>Foss : </b>".$foss."</li><li><b>Language : </b>".$language."</li></ul>";
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	}else{
		$output = "";
	}
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
	$form['foss_category'] = array(
		'#type' => 'select',
		'#title' => t(''),
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
	
	$form['language'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $lang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<td>',
		'#suffix' => '</td>'
	);
	$form['save'] = array(
		'#type' => 'submit',
		'#value' => 'Search',
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table></div><div class="list_data">'.$output.'</div>',
		'#attributes' => array('class' => 'form-select locate_tutorial')
	);
	
	return $form;
}

function update_tutorial_search_form_validate($form, &$form_state){

	if($form_state['values']['foss_category'] == '')
		form_set_error('foss_category', t('Please select FOSS category'));
	if($form_state['values']['language'] == '')
		form_set_error('language', t('Please select language'));
}

function update_tutorial_search_form_submit($form, &$form_state){
	
	$redirect = 'stupload/update_tutorial_search/?foss='.$form_state['values']['foss_category'].'&language='.$form_state['values']['language'];
	drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
}

