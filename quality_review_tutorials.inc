<?php

function quality_review_tutorials(){

	global $user;
	$flag = 0;
	$flag1 = 0;
	$query = "";
	$sql_count = "";
	$sfoss = '';
	$slang = '';
	$lang_query = '';
	// get language
	$query ="select tl.name from tutorial_languages tl, tutorial_quality_roles tqr where tqr.language_id=tl.id and tqr.uid=".$user->uid." order by tl.name ASC";
	$result = db_query($query);
	$lang[''] = 'Select Language';
	while($row = db_fetch_object($result)){
		//var_dump($row);
		if($flag1 > 0){
			$lang_query .= ' or ';
		}
		$lang_query .= "tr.language ='".$row->name."'";
		$lang[$row->name] = $row->name;
		$flag1++;
	}
	//var_dump($lang);
	if(user_access('quality review tutorial')){
		$where_query ='';			
		if(isset($_REQUEST['foss']) && isset($_REQUEST['lang']) && isset($_REQUEST['status'])){
			$sfoss = $_REQUEST['foss'];
			$slang = $_REQUEST['lang'];
			$status = $_REQUEST['status'];
			$where_query = "and tr.language='".$_REQUEST['lang']."' and td.foss_category='".$_REQUEST['foss']."' and (tr.tutorial_outline_status = ".$status." or tr.tutorial_script_status=".$status." or tutorial_video_status=".$status." or tcc.tutorial_slide_status=".$status." or tcc.tutorial_code_status=".$status." or tcc.tutorial_assignment_status=".$status.")";
		}elseif(isset($_REQUEST['foss']) && isset($_REQUEST['lang'])){
			$sfoss = $_REQUEST['foss'];
			$slang = $_REQUEST['lang'];
			$where_query = "and tr.language='".$_REQUEST['lang']."' and td.foss_category='".$_REQUEST['foss']."'";
		}elseif(isset($_REQUEST['foss']) && isset($_REQUEST['status'])){
			$sfoss = $_REQUEST['foss'];
			$status = $_REQUEST['status'];
			$where_query = "and td.foss_category='".$_REQUEST['foss']."' and (tr.tutorial_outline_status = ".$status." or tr.tutorial_script_status=".$status." or tutorial_video_status=".$status." or tcc.tutorial_slide_status=".$status." or tcc.tutorial_code_status=".$status." or tcc.tutorial_assignment_status=".$status.") and (".$lang_query.")";
		}elseif(isset($_REQUEST['lang']) && isset($_REQUEST['status'])){
			$slang = $_REQUEST['lang'];
			$status = $_REQUEST['status'];
			$where_query = "and tr.language='".$_REQUEST['lang']."' and (tr.tutorial_outline_status = ".$status." or tr.tutorial_script_status=".$status." or tutorial_video_status=".$status." or tcc.tutorial_slide_status=".$status." or tcc.tutorial_code_status=".$status." or tcc.tutorial_assignment_status=".$status.")";
		}elseif(isset($_REQUEST['foss'])){
			$sfoss = $_REQUEST['foss'];
			$where_query = "and td.foss_category='".$_REQUEST['foss']."' and (".$lang_query.")";
		}elseif(isset($_REQUEST['lang'])){
			$slang = $_REQUEST['lang'];
			$where_query = "and tr.language='".$_REQUEST['lang']."'";
		}elseif(isset($_REQUEST['status'])){
			$status = $_REQUEST['status'];
			$where_query = "and (tr.tutorial_outline_status = ".$status." or tr.tutorial_script_status=".$status." or tutorial_video_status=".$status." or tcc.tutorial_slide_status=".$status." or tcc.tutorial_code_status=".$status." or tcc.tutorial_assignment_status=".$status.") and (".$lang_query.")";
		}else{
			$where_query = "and (".$lang_query.")";
		}		
	}
	//var_dump($_REQUEST['status']);

	if(isset($_REQUEST['status']) && $_REQUEST['status'] == 7){
		$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status = 'public_review' ".$where_query;
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status = 'public_review' ".$where_query;
	}else{
	    $query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and tr.tutorial_status !='hold' ".$where_query;
	$sql_count = "select count(*) from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.tutorial_detail_id=td.id and tr.tutorial_content_id=tcc.id and tr.tutorial_status !='public_review' and tr.tutorial_status !='accepted' and tr.tutorial_status !='hold' ".$where_query;
	}
	//var_dump($query);
	//die;
	//i$result = pager_query($query, 30, 0, $sql_count);
	$header = array(
		array('data'=>'Sno', 'field'=>'td.order_code', 'sort' => 'td.order_code asc'),
		array('data'=>'Foss', 'field'=>'td.foss_category'),
		array('data'=>'Lang', 'field'=>'td.language'),
		array('data'=>'Tutorial Name', 'field'=>'td.tutorial_name'),
		array('data'=>'Outline', 'field'=>'tr.tutorial_outline_status'),
		array('data'=>'Script', 'field'=>'tr.tutorial_script_status'),
		array('data'=>'Video', 'field'=>'td.tr.tutorial_video_status'),
		array('data'=>'Slide', 'field'=>'tcc.tutorial_slide_status'),
		array('data'=>'Code', 'field'=>'tcc.tutorial_code_status'),
		array('data'=>'Assign', 'field'=>'tcc.tutorial_assignment_status'),
		array('data'=>'Action')
	);
	$query .= tablesort_sql($header);
	//var_dump($query);
	//die;
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$sno = 1;
	//$header = array('Sno', 'Level', 'Tutorial Name', 'Outline', 'Script', 'Video', 'Slides', 'Code', 'Assignment', 'Action');
	while ($item = db_fetch_object($result)) {
		$link_str = "";
		if($flag == 0)
			if($status !=''){
				$link_str = "<form action='".$GLOBALS['base_url']."/stupload/quality_review_tutorial_page' method='post'><input type='hidden' name='trid' value='".$item->trid."'><input type='hidden' name='status' value=".$status."><input type='submit' value='Review'></form>";
			}else{
				$link_str = "<form action='".$GLOBALS['base_url']."/stupload/quality_review_tutorial_page' method='post'><input type='hidden' name='trid' value='".$item->trid."'><input type='hidden' name='status' value='all'><input type='submit' value='Review'></form>";
			}
			//$link_str = 'stupload/review_tutorial_page?trid='.$item -> trid."'>Review</a>";
		else{
			$link_str = 'stupload/play_video?tr='.$item -> trid."'>View</a>";
		}
		$row[] = array($sno++, $item -> foss_category, $item -> language, $item -> tutorial_name, component_status_symbols($item->tutorial_outline_status), component_status_symbols($item->tutorial_script_status),component_status_symbols($item->tutorial_video_status), component_status_symbols($item->tutorial_slide_status), component_status_symbols($item->tutorial_code_status),component_status_symbols($item->tutorial_assignment_status), $link_str);
		
	}
	
	if('quality review tutorial'){
		$output = drupal_get_form('quality_list_tutorials_form', $flag, $sfoss, $slang, $status, $lang);
	}
	
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	}else{
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	
	return $output;
}

// quality review
function quality_list_tutorials_form($form_state, $flag, $sfoss, $slang, $sstatus, $lang){
	
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
		'#options' => $foss_list,
		'#default_value' => $sfoss,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div style="margin: auto;"><table class="foss_search" align="Center" style="border-collapse: inherit; width: 70%;" cellpadding="7"><tr><td>',
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
	$form['status'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' =>  array('' => 'Status', 0 => 'Pending', 1 => 'Under admin review', 2 => 'Under Domain review', 3 => 'Under quality check', 4 => 'Accepted', 5 => 'Need improvement', 6 => 'Not required', 7 => 'Public Review'),
		'#default_value' => $sstatus,
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

function quality_list_tutorials_form_validate($form, &$form_state){

}

function quality_list_tutorials_form_submit($form, &$form_state){

	
	$redirect = 'stupload/quality_review_tutorials';
	
	if($form_state['values']['foss'] != '' && $form_state['values']['lang'] != '' && $form_state['values']['status'] != ''){
		$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang'], 'status'=> $form_state['values']['status'] );
	}elseif($form_state['values']['foss'] != '' && $form_state['values']['lang'] != ''){
		$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang']);
	}elseif($form_state['values']['foss'] != '' && $form_state['values']['status'] != ''){
		$query = array('foss' => $form_state['values']['foss'], 'status'=> $form_state['values']['status'] );
	}elseif($form_state['values']['lang'] != '' && $form_state['values']['status'] != ''){
		$query = array('lang'=> $form_state['values']['lang'], 'status'=> $form_state['values']['status'] );
	}elseif($form_state['values']['foss'] != ''){
		$query = array('foss' => $form_state['values']['foss']);
	}elseif($form_state['values']['lang'] != ''){
		$query = array('lang'=> $form_state['values']['lang']);
	}elseif($form_state['values']['status'] != ''){
		$query = array('status'=> $form_state['values']['status'] );
	}else{
		$query = '';
	}

	drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
}

//Review tutorial page
function quality_review_tutorial_page() {
	$path = drupal_get_path('module', 'stupload');
	drupal_add_js($path . '/stupload.js');
	drupal_add_css($path . '/stupload.css');
	drupal_add_js($path . '/video.js');
	drupal_add_css($path . '/video-js.css');
	global $user;
	if($_POST['trid']){
		$query = "select tr.id as trid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_status, tr.tutorial_status, tcc.tutorial_slide, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_status from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$_POST['trid']."'";
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
					$video_info .= '<td><b>Bit Rate:</b> '.$bitrate.'/s</td></tr></table></p>';
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
				if($_POST['status'] == 'all'){
					if($tutorial->language == 'English'){
                                           if($tutorial->tutorial_slide_status !=4)
                                               $comp[3] = 'Slide';
                                           if($tutorial->tutorial_code_status !=4 )
                                               $comp[5] = 'Codefile';
                                           if($tutorial->tutorial_assignment_status !=4)
                                               $comp[6] = 'Assignment';
                                        }
                                        if($tutorial->tutorial_outline_status !=4)
                                                $comp[1] = 'Outline';
                                        if($tutorial->tutorial_script_status !=4)
                                                $comp[2] = 'Script';
                                        if($tutorial->tutorial_video_status !=4)
                                                $comp[4] = 'Video';
                                        if(count($comp) > 1)
                                                $comp [0] = 'All';
                                }else{
                                    if($tutorial->language == 'English'){
                                        if($tutorial->tutorial_slide_status == $_POST['status'])
                                            $comp[3] = 'Slide';
                                        if($tutorial->tutorial_code_status == $_POST['status'])
                                            $comp[5] = 'Codefile';
                                    	if($tutorial->tutorial_assignment_status == $_POST['status'])
                                            $comp[6] = 'Assignment';
                                    }
                                    if($tutorial->tutorial_outline_status == $_POST['status'])
                                            $comp[1] = 'Outline';
                                    if($tutorial->tutorial_script_status == $_POST['status'])
                                            $comp[2] = 'Script';
                                    if($tutorial->tutorial_video_status == $_POST['status'])
                                            $comp[4] = 'Video';
                                    if(count($comp) > 1)
                                            $comp [0] = 'All';
                                }
				//var_dump($comp);
				//var_dump($_POST['status']);
				//die;
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
				// calculate publishing option
				$query = "select tr.id as trid from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id=".$tutorial->trid." and tr.tutorial_outline_status = 4 and tr.tutorial_script_status=4 and tutorial_video_status=4 and (tcc.tutorial_slide_status=4 || tcc.tutorial_slide_status=6) and (tcc.tutorial_code_status=4 || tcc.tutorial_code_status=6) and (tcc.tutorial_assignment_status=4 || tcc.tutorial_assignment_status=6)";
				$result = db_query($query);
				$row = db_fetch_object($result);
				$options = array(4 => 'Accepted', 5 => 'Need Improvement');
				if($row){
					$options = array(5 => 'Need Improvement', 'accepted' => 'Publish the tutorial');
					if($tutorial->language != 'English'){
						$options = array(5 => 'Need Improvement', 'accepted' => 'Publish the tutorial', 'public_review' => 'Public Review');
					}
				}
				$output .= drupal_get_form('quality_review_tutorial_form', $tutorial->trid, '', $comp, $options);		
				$output .= "</div>";
				$output .= "<div class='left-content'>";
					$output .= "<div class='summery-outline'>";
						$output .= "<p class='summery-title'>Outline: ".component_status($tutorial->tutorial_outline_status, $tutorial->tutorial_outline_uid, $tutorial->tutorial_outline)."</p>";
						$output .= ".$outline.";
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
					$output .= "<p class='summery-title'>Script: ".component_status($tutorial->tutorial_script_status, $tutorial->tutorial_script_uid, $tutorial->tutorial_script)."</p>";
					$output .= ".$script.";
				$output .= "</div>";
			$output .= "</div>";
		$output .= "</div>";
		return $output;
	}else{
		$output = "<p>Something went wrong. Please, try again.</p>";
		return $output;
	}
}

function quality_review_tutorial_form($form_state, $trid = NULL, $status = NULL, $comp = NULL, $options){
		// add js file 

		global $user;
		
		/*$query = "select tr.id as trid from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id=".$trid." and tr.tutorial_outline_status = 4 and tr.tutorial_script_status=4 and tutorial_video_status=4 and (tcc.tutorial_slide_status=4 || tcc.tutorial_slide_status=6) and (tcc.tutorial_code_status=4 || tcc.tutorial_code_status=6) and (tcc.tutorial_assignment_status=4 || tcc.tutorial_assignment_status=6)";
		$result = db_query($query);
		$row = db_fetch_object($result);
		if($row){
			$options = array(5 => 'Need Improvement', 'accepted' => 'Publish the tutorial', 'public_review' => 'Public Review');
			
		}else{
			$options = array(4 => 'Accepted', 5 => 'Need Improvement');
		} */
				
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
			'#validated' => TRUE,
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
	

	function quality_review_tutorial_form_submit($form, &$form_state){	
	
		global $user;
		$com = '';
		$flag = 0;
		$component = unserialize($_POST['comp']);
		if($_POST['status'] == 4){
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					if($flag > 0){
						$com .= ', '; 
					}
					$com .=status_update_comp_query($c, 4);
					$flag++;					
				}
			}else{
				$com = status_update_comp_query($component[$_POST['component']], 4);
			}
			$query = "update tutorial_resources tr, tutorial_common_contents tcc set tr.reviewer='".$user->name."', ".$com." where tr.id=".$_POST['trid']." and tr.tutorial_content_id = tcc.id";	
		}elseif($_POST['status'] == 5){
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
			$query = "update tutorial_resources tr, tutorial_common_contents tcc set tr.reviewer='".$user->name."', ".$com." where tr.id=".$_POST['trid']." and tr.tutorial_content_id = tcc.id";	
		}elseif($_POST['status'] == 'accepted'){
			$query = "update tutorial_resources set upload_time=CURRENT_TIMESTAMP ,reviewer='".$user->name."', tutorial_status='accepted' where id=".$_POST['trid'];
		}elseif($_POST['status'] == 'public_review'){
			$query = "update tutorial_resources tr, tutorial_common_contents tcc set tr.upload_time=CURRENT_TIMESTAMP, tr.reviewer='".$user->name."', tr.tutorial_outline_status = 7, tr.tutorial_script_status = 7, tr.tutorial_video_status = 7, tr.tutorial_status='public_review' where tr.id=".$_POST['trid']." and tr.tutorial_content_id = tcc.id";
		}
		
		db_query($query);
		
		// insert into the comment improvement table
		if($_POST['comment'] != ''){
			$_POST['comment']=db_escape_string($_POST['comment']);
			$query = "INSERT INTO `comment_improvement` (`id`, `uid`, `trid`, `reviewed_time`, `comment`) VALUES (NULL, '".$user->uid."', '".$_POST['trid']."', CURRENT_TIMESTAMP, '".$_POST['comment']."')";
			db_query($query);
		}

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$_POST['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'review')";

		db_query($query);		
		// Get the Tutorial Details
		$query = "select td.foss_category, td.tutorial_name, td.tutorial_level , tr.language, tr.tutorial_outline_uid as Outline, tr.tutorial_script_uid as Script, tr.tutorial_video_uid as Video, tcc.tutorial_slide_uid as Slide, tcc.tutorial_code_uid as Codefile, tcc.tutorial_assignment_uid as Assignment from tutorial_resources tr,  tutorial_details td, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.id=".$_POST['trid']." and tcc.id=tutorial_content_id";
		$result = db_query($query);
		
		$tutorial_detail = db_fetch_object($result);

		// Email
		if($_POST['status'] == 4){
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					$a = new stdClass();
					$a->tc = $c;
					$a->stage = "qreview";
					$a->status = "Accepted";					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
					$ud = user_email($tutorial_detail->$c);
					$mailto = $ud->mail;
					$name = $ud->name;
					$role = variable_get('contributor', '');
					$message = "";
					stupload_notify($mailto, $role, $message, $tds, $name);				
				}
			}else{
				$a = new stdClass();
				$a->tc = $component[$_POST['component']];
				$a->stage = "qreview";
				$a->status = "Accepted";			
				$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				$ud = user_email($tutorial_detail->$component[$_POST['component']]);
				
				$mailto = $ud->mail;
				$name = $ud->name;
			    $role = variable_get('contributor', '');
			    $message = "";
				stupload_notify($mailto, $role, $message, $tds, $name);
			}			
		}else if($_POST['status'] == 5){
			if($_POST['component'] == 0){
				unset($component[0]);
				foreach($component as $c){
					$a = new stdClass();
					$a->tc = $c;
					$a->stage = "qreview";
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
				$a->stage = "qreview";
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
		drupal_goto('stupload/quality_review_tutorials', '', $fragment = NULL, $http_response_code = 302);
	}

?>



