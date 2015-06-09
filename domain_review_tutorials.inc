<?php

function domain_review_tutorials() {
	global $user;
	$flag = 0;
	$flag1 = 0;
	$query = "";
	$sql_count = "";
	$sfoss = '';
	$slang = '';
	$lang_query = '';
	// get language
	$query ="select tl.name from tutorial_languages tl, tutorial_domain_reviewer_roles tdr where tdr.language_id=tl.id and tdr.uid=".$user->uid." order by tl.name ASC";
	$result = db_query($query);
	$lang[''] = 'Select Language';
	while($row = db_fetch_object($result)) {
		if($flag1 > 0) {
			$lang_query .= ' or ';
		}
		$lang_query .= "tr.language ='".$row->name."'";
		$lang[$row->name] = $row->name;
		$flag1++;
	}

	if(user_access('domain review tutorial')) {	
		if(isset($_REQUEST['foss']) && isset($_REQUEST['lang'])) {
			$sfoss = $_REQUEST['foss'];
			$slang = $_REQUEST['lang'];
			$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.language='".$_REQUEST['lang']."' and td.foss_category='".$_REQUEST['foss']."' and ((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=2) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1))";
			$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.language='".$_REQUEST['lang']."' and td.foss_category='".$_REQUEST['foss']."' and ((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=2) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1))";
		}else {
			$query ="select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and (".$lang_query.") and (((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=2) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1)))";
			$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and (".$lang_query.") and (((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=2) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1)))";
		}
	}
	//var_dump('ss');
	//$result = pager_query($query, 30, 0, $sql_count);
	$header = array(
		array('data'=>'Sno'),
		array('data'=>'Foss', 'field'=>'td.foss_category', 'sort'=>'asc'),
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
	//$header = array('Sno', 'Level', 'Tutorial Name', 'Outline', 'Script', 'Video', 'Slides', 'Code', 'Assignment', 'Action');
	while ($item = db_fetch_object($result)) {
		$link_str = "";
		if($flag == 0)
				$link_str = "<form action='".$GLOBALS['base_url']."/stupload/domain_review_tutorial_page' method='post'><input type='hidden' name='trid' value='".$item->trid."'><input type='submit' value='Review'></form>";
			//$link_str = 'stupload/review_tutorial_page?trid='.$item -> trid."'>Review</a>";
		else {
			$link_str = 'stupload/play_video?tr='.$item -> trid."'>View</a>";
		}
		$row[] = array($sno++, $item -> foss_category, $item -> tutorial_name, component_status_symbols($item->tutorial_outline_status), component_status_symbols($item->tutorial_script_status),component_status_symbols($item->tutorial_video_status), component_status_symbols($item->tutorial_slide_status), component_status_symbols($item->tutorial_code_status),component_status_symbols($item->tutorial_assignment_status), $link_str);
		
	}

	$output = drupal_get_form('domain_list_tutorials_form', $flag, $sfoss, $slang, $lang);		
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}

function domain_list_tutorials_form($form_state, $flag, $sfoss, $slang, $lang){
	global $user;
	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/stupload-view.css');
	
/*	//fetching list of foss category details
	$query = "select distinct foss_category from tutorial_details ORDER BY foss_category ASC";
	$result = db_query($query);
	$foss_list = array();
	$foss_list[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss_list[$row->foss_category] = $row->foss_category;
	}*/
		drupal_add_js($path.'/filter_videos_domain.js');
	$query_domain ="select tl.name from tutorial_languages tl, tutorial_domain_reviewer_roles tdr where tdr.language_id=tl.id and tdr.uid=".$user->uid." order by tl.name ASC";
	$result_domain = db_query($query_domain); 
	
	/*$domain_lang = db_fetch_object($result_domain);
	$query1 = "select DISTINCT td.foss_category from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.language='".$domain_lang->name."' and ((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=2) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1))";*/

	$flag1 = 0;
	while($domain_lang = db_fetch_object($result_domain)) {
		if($flag1 > 0) {
			$lang_query .= ' or ';
		}
		$lang_query .= "tr.language ='".$domain_lang->name."'";
		$lang[$row->name] = $row->name;
		$flag1++;
	}
	$query1 = "select DISTINCT td.foss_category from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and (".$lang_query.") and ((tr.tutorial_outline_status = 2 or tr.tutorial_outline_status = 1) or (tr.tutorial_script_status=2 or tr.tutorial_script_status=1) or tr.tutorial_video_status=2 or (tcc.tutorial_slide_status=2 or tcc.tutorial_slide_status=1) or (tcc.tutorial_code_status=2 or tcc.tutorial_code_status=1) or (tcc.tutorial_assignment_status=2 or tcc.tutorial_assignment_status=1))";
	
	$result = db_query($query1);
	$foss_list = array();
	$foss_list[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss_list[$row->foss_category] = $row->foss_category;
	}

	
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['foss'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $foss_list,
		'#default_value' => $sfoss,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div style="width: 70%; margin: auto;"><table class="foss_search" align="Center" style="border-collapse: inherit; width: 70%;" cellpadding="7"><tr><td>',
		'#suffix' => '</td>'
	);
	
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
		'#attributes' => array('class' => 'form-select locate_tutorial'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table></div>'
	);
	
	return $form;
}

function domain_list_tutorials_form_validate($form, &$form_state){

	if($form_state['values']['foss'] == '')
		form_set_error('foss', t('Please select FOSS category'));
	if($form_state['values']['lang'] == '')
		form_set_error('lang', t('Please select language'));
}

function domain_list_tutorials_form_submit($form, &$form_state){
	$redirect = 'stupload/domain_review_tutorials';
	$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang'] );
	drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
}

//Review tutorial page
function domain_review_tutorial_page() {
	$path = drupal_get_path('module', 'stupload');
	drupal_add_js($path . '/stupload.js');
	drupal_add_css($path . '/stupload.css');
	drupal_add_js($path . '/video.js');
	drupal_add_css($path . '/video-js.css');
	global $user;
	
	if($_REQUEST['trid']){
		$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tr.tutorial_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$_REQUEST['trid']."'";
		$result = db_query($query);
		$tutorial = db_fetch_object($result);
		// show current status
		// Outline
		$outline = component_status($tutorial->tutorial_outline_status, $tutorial->tutorial_outline_uid, $tutorial->tutorial_outline, 1);

		// Script
		$script = 'pending';
		if($tutorial->tutorial_script != 'pending')
			$script = "<iframe width='100%' height='600px' src='".variable_get('wiki_path', '').$tutorial->tutorial_script."'></iframe>";
		// Video
		if($tutorial->tutorial_video != 'pending' && file_exists(variable_get('file_upload_path', '').$tutorial->tutorial_video)){
			$file_ext = substr($tutorial->tutorial_video, strripos($tutorial->tutorial_video, '.'));
			$movie = new ffmpeg_movie(variable_get('file_upload_path', '').$tutorial->tutorial_video);
			$height = $movie->getFrameHeight();
			$width = $movie->getFrameWidth();
			$time = gmdate("H:i:s", $movie->getDuration());
			$bitrate = $movie->getBitRate();
			$fps = $movie->getFrameRate();
			$size = filesize(variable_get('file_upload_path', '').$tutorial->tutorial_video);
			$video_info = '<p><table border"0" cellpadding="4" style="width: 100%; border-collapse: inherit;"><tr><td><b>File Size:</b> '._format_bytes($size).'</td>';
			$video_info .= '<td><b>Video Timing: </b>'.$time.'</td></tr>';
			$video_info .= '<tr><td><b>Frame Width:</b> '.$width.'</td>';
			$video_info .= '<td><b>Frame Height:</b> '.$height.'</td></tr>';
			$video_info .= '<tr><td><b>Frame Rate(fps):</b> '.$fps.'</td>';
			$video_info .= '<td><b>Bit Rate:</b> '.$bitrate.'/bps</td></tr></table></p>';
			$video ='<video id="example_video_1" class="video-js vjs-default-skin" controls width="700" height="500" data-setup="{}">';
			if($file_ext == '.mp4'){
            	$video .='<source src="'.variable_get('tutorial_path', '').$tutorial->tutorial_video.'" type="video/mp4" />';
            }else{
            	$video .='<source src="'.variable_get('tutorial_path', '').$tutorial->tutorial_video.'" type="video/ogg" />';
            }
            $video .= '</video>';
		}else{
			$video = $tutorial->tutorial_video;
		}
		$slide = component_status($tutorial->tutorial_slide_status, $tutorial->tutorial_slide_uid, $tutorial->tutorial_slide);
		$codefile = component_status($tutorial->tutorial_code_status, $tutorial->tutorial_code_uid, $tutorial->tutorial_code);
		$assign = component_status($tutorial->tutorial_assignment_status, $tutorial->tutorial_assignment_uid, $tutorial->tutorial_assignment);
		
		// component shan
		$comp = array();
		if($tutorial->tutorial_code_status == 2 || $tutorial->tutorial_code_status == 1)
			$comp[5] = 'Codefile'; 
		if($tutorial->tutorial_assignment_status == 2 || $tutorial->tutorial_assignment_status == 1)
			$comp[6] = 'Assignment';
		if($tutorial->tutorial_outline_status == 2 || $tutorial->tutorial_outline_status == 1) 
			$comp[1] = 'Outline'; 
		if($tutorial->tutorial_script_status == 2 || $tutorial->tutorial_script_status == 1)
			$comp[2] = 'Script'; 
		if($tutorial->tutorial_slide_status == 2 || $tutorial->tutorial_slide_status == 1)
			$comp[3] = 'Slide';
		if($tutorial->tutorial_video_status == 2)
			$comp[4] = 'Video'; 
		if(count($comp) > 1){
			$comp [0] = 'All';
		}
		$output = "<div class='stupload-form'>";
			$output .= "<div class='summery'>";
				$output .= "<div class='summery-title-content'>";				
				$output .="<span class='left'>Foss Category</span><span class='right'>: ".$tutorial->foss_category."</span>";
				$output .="<span class='left'>Name</span><span class='right'>: ".$tutorial->tutorial_name."</span>";
				$output .="<span class='left'>Level</span><span class='right'>: ".$tutorial->tutorial_level."</span>";
				$output .="<span class='left'>Language</span><span class='right'>: ".$tutorial->language."</span>";
				$output .="<span class='left'>Tutorial overall Status</span><span class='right'>: ".$tutorial->tutorial_status."</span>";
				//Previous Remarks for the tutorial
				$query = "select u.name, ci.reviewed_time, ci.comment from comment_improvement ci, users u where ci.trid='".$_POST['trid']."' and u.uid=ci.uid order by ci.reviewed_time DESC";
				$result = db_query($query);
				$tmp_comment = " - ";
				while($row = db_fetch_object($result)){
					if($tmp_comment == " - "){
						$tmp_comment = '';
					}
					$tmp_comment .= "<p> ".$row->name."  ".$row->reviewed_time."<br />".stuolpad_outline_humen_view($row->comment)."</p>";
				}
				$output .="<span class='left'>Remarks </span><span class='right'>".$tmp_comment."</span>";
				// Get form
				$output .= drupal_get_form('domain_review_tutorial_form', $tutorial->trid, '', $comp);		
				$output .= "</div>";
				$output .= "<div class='left-content'>";
					$output .= "<div class='summery-outline'>";
						$output .= "<p class='summery-title' style='float: left; margin-right: 80px;'>Outline : </p>";
						// update outline form
						
						if($tutorial->language == 'English')
						    $link_str = "<form  action='".$GLOBALS['base_url']."/stupload/update_english_tutorial/outline' method='post'><input type='hidden' name='outline' value='".$tutorial->tutorial_outline."'><input type='hidden' name='trid' value='".$tutorial->trid."'><input type='hidden' name='tn' value='".$tutorial->tutorial_name."'><input type='submit' value='Update' style='margin-left: 0px;'><input type='hidden' name='redirection' value='3'></form>";
						else
						    $link_str = "<form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/outline' method='post'><input type='hidden' name='outline' value='".$tutorial->tutorial_outline."'><input type='hidden' name='trid' value='".$tutorial->trid."'><input type='hidden' name='tn' value='".$tutorial->tutorial_name."'><input type='submit' value='Update' style='margin-left: 0px;'><input type='hidden' name='redirection' value='3'></form>";
						$output .= "<p>".$link_str."</p>";
						$output .= $outline;
					$output .= "</div>";
					$output .= "<div class='summery-slide'>";
						$output .= "<p class='summery-title'>Slide";
						$output .= ": ".$slide."</p>";
					$output .= "</div>";
					$output .= "<div class='summery-codefile'>";
						$output .= "<p class='summery-title'>Codefile";
						$output .= ": ".$codefile."</p>";
					$output .= "</div>";
					$output .= "<div class='summery-assignment'>";
						$output .= "<p class='summery-title'>Assignment";
						$output .= ": ".$assign."</p>";
					$output .= "</div>";
				$output .= "</div>";
				$output .= "<div class='right-content'>";
					$output .= "<div class='summery-video'>";
						$output .= "<p class='summery-title'>Video: ".component_status($tutorial->tutorial_video_status, $tutorial->tutorial_video_uid, $tutorial->tutorial_video)." </p>";
						$output .= $video_info;
						//$output .= "<p class='summery-content'>".$video."</p>";
					$output .= "</div>";
				$output .= "</div>";
				$output .= "<div class='summery-script'>";
				$output .= "<p class='summery-content'>".$video."</p>";
				$output .= "</div>";
				$output .= "<div class='summery-script'>";
					$output .= "<p class='summery-title'>Script";
					$output .= ": ".$script."</p>";
				$output .= "</div>";
			$output .= "</div>";
		$output .= "</div>";
		return $output;
	}else{
		$output = "<p>Something went wrong. Please, try again.</p>";
		return $output;
	}


}

function domain_review_tutorial_form($form_state, $trid, $status, $comp) {
		// add js file 
		global $user;
		if(user_access('domain review tutorial')) {			
			$options = array(3 => 'Accepted', 5 => 'Need Improvement');
		}
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['trid'] = array(
			'#type' => 'hidden',
			'#title' => t('id'),
			'#required' => TRUE,
			'#value' => $trid
		);
		
		$form['comp'] = array(
			'#type' => 'hidden',
			'#value' => serialize($comp)
		);
			
		$form['component'] = array(
			'#type' => 'select',
			'#title' => "Component",
			'#required' => TRUE,
			'#default_value' => '',
			'#options' => $comp,
			'#attributes' => array('class' => 'reviewer-status')
		);
		$form['status'] = array(
			'#title' => "Status",
			'#type' => 'radios',
			'#required' => TRUE,
			'#default_value' => $status,
			'#options' => $options,
			'#attributes' => array('class' => 'reviewer-status')
		);

		$form['comment'] = array(
			'#type' => 'textarea',
			'#required' => FALSE,
			'#title' =>t('Remarks'),
			'#prefix' => "<div class='reviewer-comment'>",
			'#suffix' => "</div>"
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Change')
	   );
	
	return $form;


	}
	

	function domain_review_tutorial_form_submit($form, &$form_state){

		global $user;
		$com = '';
		$flag = 0;
		if($_POST['status'] == 3){
			$component = unserialize($_POST['comp']);
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					if($flag > 0){
						$com .= ', '; 
					}
					$com .=status_update_comp_query($c, 3);
					$flag++;					
				}
			}else{
				$com = status_update_comp_query($component[$_POST['component']], 3);
			}
		}else{
			$component = unserialize($_POST['comp']);
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					if($flag > 0){
						$com .= ', '; 
					}
					$com .=status_update_comp_query($c, 5);
					$flag++;					
				}
			}else{
				$com = status_update_comp_query($component[$_POST['component']], 5);
			}
		}		

		$query = "update tutorial_resources tr, tutorial_common_contents tcc set tr.reviewer='".$user->name."', ".$com." where tr.id=".$_POST['trid']." and tr.tutorial_content_id = tcc.id";	
		db_query($query);
		
		
		// Get the Tutorial Details
		$query = "select tr.language, tr.uid, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$_POST['trid']."'";
		$result = db_query($query);
		$tutorial_detail = db_fetch_object($result);

		$a = new stdClass();
		$a->tc = 'Review';					
		$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);

		// insert into the comment improvement table
		if($_POST['comment'] != ''){
			$_POST['comment']=db_escape_string($_POST['comment']);
			$query = "INSERT INTO `comment_improvement` (`id`, `uid`, `trid`, `reviewed_time`, `comment`) VALUES (NULL, '".$user->uid."', '".$_POST['trid']."', CURRENT_TIMESTAMP, '".$_POST['comment']."')";

			db_query($query);
		}

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$_POST['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'review')";

		db_query($query);
		
		//Email
		$query = "select td.foss_category, td.tutorial_name, td.tutorial_level , tr.language, tr.tutorial_outline_uid as Outline, tr.tutorial_script_uid as Script, tr.tutorial_video_uid as Video, tcc.tutorial_slide_uid as Slide, tcc.tutorial_code_uid as Codefile, tcc.tutorial_assignment_uid as Assignment from tutorial_resources tr,  tutorial_details td, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.id=".$_POST['trid']." and tcc.id=tutorial_content_id";
		$result = db_query($query);
		
		$tutorial_detail = db_fetch_object($result);
		
		if($_POST['status'] == 3){
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					$a = new stdClass();
					$a->tc = $c;
					$a->stage = "dreview";
					$a->status = "Accepted";
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
					$ud = user_email($tutorial_detail->$c);
					$mailto = $ud->mail;
					$name = $ud->name;
					$role = variable_get('contributor', '');
					$message = "";
					stupload_notify($mailto, $role, $message, $tds, $name);
					//send email to quality reviewer
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					stupload_notify($quality_mailid, 'quality_checker', $message, $tds, $name);
				}
			}else{
				$a = new stdClass();
				$a->tc = $component[$_POST['component']];
				$a->stage = "dreview";
				$a->status = "Accepted";
				$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				$ud = user_email($tutorial_detail->$component[$_POST['component']]);
				$mailto = $ud->mail;
				$name = $ud->name;
			    $role = variable_get('contributor', '');
			    $message = "";
				stupload_notify($mailto, $role, $message, $tds, $name);
				//send email to quality reviewer
				$quality_mailid = get_language_role_emails($tutorial_detail->language);
				stupload_notify($quality_mailid, 'quality_checker', $message, $tds, $name);
			}
			
		}else if($_POST['status'] == 5){
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					$a = new stdClass();
					$a->tc = $c;
					$a->stage = "dreview";
					$a->status = "Need Improvement";
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
					$ud = user_email($tutorial_detail->$c);
					$mailto = $ud->mail;
					$name = $ud->name;
					$role = variable_get('contributor', '');
					$message = $_POST['comment'];
					stupload_notify($mailto, $role, $message, $tds, $name);
				}
			}else{
				$a = new stdClass();
				$a->tc = $component[$_POST['component']];
				$a->stage = "dreview";
				$a->status = "Need Improvement";
				$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				$ud = user_email($tutorial_detail->$component[$_POST['component']]);
				$mailto = $ud->mail;
				$name = $ud->name;
			    $role = variable_get('contributor', '');
			    $message = $_POST['comment'];
				stupload_notify($mailto, $role, $message, $tds, $name);
			}
		}
		drupal_set_message('Status updated successfully');
		drupal_goto('stupload/domain_review_tutorials', '', $fragment = NULL, $http_response_code = 302);
		
	}	



