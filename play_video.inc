<?php
function play_video(){
	global $user;
	$tr = $_REQUEST['tr'];
	$query = "select tr.hit_count, tr.id as trid, tr.language, tr.upload_time, tr.reviewer, tr.tutorial_outline,  tr.tutorial_outline_uid, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_uid, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_uid, tr.tutorial_video_status, tr.tutorial_status, tc.tutorial_slide, tc.tutorial_slide_uid, tc.id as tcid, tc.tutorial_code, tc.tutorial_code_uid, tc.tutorial_assignment, tc.tutorial_assignment_uid, td.id as tdid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code from tutorial_resources tr, tutorial_common_contents tc, tutorial_details td where tr.id=".$tr." and tc.id=tr.tutorial_content_id and td.id=tr.tutorial_detail_id";
	$result = db_query($query);
	$value = db_fetch_object($result);

	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/video-js.css');
	drupal_add_js($path.'/tinybox.js');
	drupal_add_js($path.'/video.js');
	
	if($sval == 1){
		$foss = $value->foss_category;
		$language = $value->language;
	}elseif($sval == 2){
		$foss = "all";
		$language = "all";
	}elseif($sval == 3){
		$foss = $value->foss_category;
		$language = "all";
	}elseif($sval == 4){
		$foss = "all";
		$language = $value->language;
	}else{
		$foss = "Linux";
		$language = "English";
	}
	// get user details
	$outline_user = ($value->tutorial_outline_uid ? ("<span class='help-text' style='color:black'> - ".get_contributor_name($value->tutorial_outline_uid)."</span>") : '');
	$script_user = ($value->tutorial_script_uid ? ("<span class='help-text'  style='color:black'> - ".get_contributor_name($value->tutorial_script_uid)."</span>") : '');
	$video_user = ($value->tutorial_video_uid ? ("<span class='help-text' style='color:black'> - ".get_contributor_name($value->tutorial_video_uid)."</span>") : '');
	$slide_user = ($value->tutorial_slide_uid ? ("<span class='help-text' style='color:black'> - ".get_contributor_name($value->tutorial_slide_uid)."</span>") : '');
	$code_user = ($value->tutorial_code_uid ? ("<span class='help-text' style='color:black'> - ".get_contributor_name($value->tutorial_code_uid)."</span>") : '');
	$assign_user = ($value->tutorial_assignment_uid ? ("<span class='help-text' style='color:black'> - ".get_contributor_name($value->tutorial_assignment_uid)."</span>") : '');
	
	
	$output = '<div class="tutorial-view-page">';
	/* video */
	if ($value){
		$output .= "<div class='video-sidebar'>";
			/* tutorial details */
			//$movie = new ffmpeg_movie(variable_get('file_upload_path', '').$value->tutorial_video);
			//$time = gmdate("H:i:s", $movie->getDuration());
			//$size = filesize(variable_get('file_upload_path', '').$value->tutorial_video);
			$output .= "<h1 class='title'>".human_text($value->tutorial_name)."</h1>";
			$output .= "<table class='video-det'>";
					
					$output .= '<tr><td class="rich-text">FOSS</td><td>'.human_text($value->foss_category).'</td</tr>';
					$output .= '<tr><td class="rich-text">Level</td><td>'.$value->tutorial_level.'</td</tr>';
					$output .= '<tr><td class="rich-text">Language</td><td>'.$value->language.'</td</tr>';
					$output .= '<tr><td class="rich-text">Published</td><td>'.$value->upload_time.'</td</tr>';
			$output .= "</table>";
			$output .= "<p class='seprator nobormar'></p>";
			/* outline */
			$output .= "<div class='video-outline'>";
			    $output .= "<h2 class='sub-title'>Outline".$outline_user."</h2>";
				if ($value -> tutorial_outline_status != 0){
					$output .= "<p class='outline'>".stuolpad_outline_humen_view($value ->tutorial_outline)."</p>";
				}else{
					$output .= '<p style="text-align:center">Currently not available</p>';
				}
				$output .= "<p class='seprator nobormar'></p>";
			$output .= "</div>";
		$output .= "</div>";
		$output .= "<div class='video-details'>";
			if($value->tutorial_status == 'public_review'){
				$output .= "<p style='color:red;'>Disclaimer: These tutorials are offered on an experimental basis without reviews.  We request your inputs to complete the assessment. To rate this video <strong><a href='".base_path()."stupload/public_review/".$value->tdid."/".$value->trid."/".$time."'>click here</a></strong></p>";
			}
			$output .= '<h2 class="sub-title">Video</h2>';
			if($value->tutorial_video_status != 0){
			    $output .= '<video id="example_video_1" class="video-js vjs-default-skin" controls width="100%" height="400" data-setup="{}">
				        	<source src="'.variable_get('tutorial_path', '').$value -> tutorial_video.'" type="video/ogg" />';
			    if($value -> language != 'English'){
				    $srt_file = $value->foss_category.'/'.$value->tutorial_level.'/'.$value->tutorial_name.'/'.$value->tutorial_name.'-English.srt';
				    if(file_exists(variable_get('file_upload_path', '').$srt_file)){
					    $output .= '<track kind="captions" src="'.variable_get('tutorial_path', '').$srt_file.'" srclang="en" label="English" />';
				    }
			    }
			    $srt_file = str_replace('.ogv', '.srt', $value->tutorial_video);
			    // var_dump(variable_get('tutorial_path', '').$srt_file); die;
			    if(file_exists(variable_get('file_upload_path', '').$srt_file)){
				    $output .= '<track kind="captions" src="'.variable_get('tutorial_path', '').$srt_file.'" srclang="en" label="'.$value -> language.'" />';
			    }
			    $output .= '</video>';
			 }else{
			    $output .="<p style='text-align:center'>Currently not available</p>";
			 
			 }
			/* download */
			$output .= '<div class="download-links"><p class="dl nobormar">';
			if ($value -> tutorial_assignment != 'pending' && $value -> tutorial_assignment != 'notrequired'){
				$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'" . variable_get('tutorial_path', '').$value -> tutorial_assignment . "'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>'.$assign_user;
			}elseif ($value -> tutorial_assignment == 'pending') {
				$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>'.$assign_user;
			}else{
				$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_required'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>'.$assign_user;
			}
			if ($value -> tutorial_code != 'pending' && $value -> tutorial_code != 'notrequired'){
				//$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . variable_get('tutorial_path', '').$value -> tutorial_code . "'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>';
				$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . base_path().'list_zip?title=Code files&file_path='.$value -> tutorial_code."'" . ", 'Codefiles', 'width=450, height=300'); " . 'return false;">Code files</a>'.$code_user;
			}elseif ($value -> tutorial_code == 'pending') {
				$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>'.$code_user;
			}else{
				$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_required'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>'.$code_user;
			}
			if ($value -> tutorial_script != 'pending'){
				$output .= ' | <a href="' . variable_get('wiki_path', '').$value -> tutorial_script . '" target="_blank">Script</a>'.$script_user;
			}else{
				$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Script', 'width=600, height=400'); " . 'return false;">Script</a>'.$script_user;
			}
			if($value->language == "English"){
				if ($value -> tutorial_script_timed != 'jspending'){
			              	$output .= ' | <a href="' . variable_get('wiki_path', '').$value -> tutorial_script_timed . '" target="_blank">Script-Timed</a>';
				      }else{
		   	                $output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Script', 'width=600, height=400'); " . 'return false;">Script-Timed</a>';
			      	}
			}
			if ($value -> tutorial_slide != 'pending'){
		   		// $output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . variable_get('tutorial_path', '').$value -> tutorial_slide . "'" . ", 'Slides', 'width=600, height=400'); " . 'return false;">Slides</a>';
		   		$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . base_path().'list_zip?title=Slides&file_path='.$value -> tutorial_slide."'" . ", 'Codefiles', 'width=450, height=300'); " . 'return false;">Slides</a>'.$slide_user;
		   	}else{
		   		$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Slides', 'width=600, height=400'); " . 'return false;">Slides</a>'.$slide_user;
		   	}
			$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."data/download_video.php?trid=" .$value->trid. "', 'Slides', 'width=600, height=400'); " . 'return false;">Video</a>'.$video_user;
			$output .= '</p></div>';
	
	
			// forum and missing tutorial
			$output .= "<p class='seprator nobormar'></p>";
			
		$output .= "</div>";
	}else{
		$output .= '<tr><td style="text-align: center;">Video is currently not available</td></tr>';
	}
	$output .= "</div>";
	return $output;
}

