<?php

	function olang_index() {

		if($_REQUEST['tr']){

			global $user;
			$path = drupal_get_path('module', 'stupload');
			drupal_add_js($path . '/stupload.js');
			drupal_add_css($path . '/stupload.css');
			$trid = $_REQUEST['tr'];			

			$query = "select tr.tutorial_detail_id, tr.tutorial_content_id as tccid, tr.language, tr.tutorial_outline, tr.tutorial_outline_uid, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_uid, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_uid, tr.tutorial_video_status, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tcc.tutorial_slide, tcc.tutorial_slide_uid, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_uid, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_uid, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.id=".$trid." and td.id=tr.tutorial_detail_id and tcc.id=tr.tutorial_content_id";

			$result = db_query($query);
			$content = db_fetch_object($result);

			// tutirial status
			$outline = component_status($content->tutorial_outline_status, $content->tutorial_outline_uid, $content->tutorial_outline, 1);
			$script = component_status($content->tutorial_script_status, $content->tutorial_script_uid, $content->tutorial_script, 2);
			$video = component_status($content->tutorial_video_status, $content->tutorial_video_uid, $content->tutorial_video);
			
			$td_path = $content->foss_category.'/'.$content->tutorial_level.'/'.$content->tutorial_name.'/';
			
			$output = "<div class='stupload-form'>";
			$output .= "<div class='summery-title-content'>";
				$output .="<span class='left'>Foss Category</span><span class='right'>: ".$content->foss_category."</span>";
				$output .="<span class='left'>Tutorial Name</span><span class='right'>: ".$content->tutorial_name."</span>";
				$output .="<span class='left'>Level</span><span class='right'>: ".$content->tutorial_level."</span>";
				$output .="<span class='left'>Language</span><span class='right'>: ".$content->language."</span>";
				$output .="<span class='left'>Study Plan Code</span><span class='right'>: ".$content->order_code."</span>";
				//Previous Remarks for the tutorial
				$query = "select u.name, ci.reviewed_time, ci.comment from comment_improvement ci, users u where ci.trid='".$trid."' and u.uid=ci.uid order by ci.reviewed_time DESC";
				$result = db_query($query);
				$tmp_comment = " - ";
				while($row = db_fetch_object($result)){
					if($tmp_comment == " - "){
						$tmp_comment = '';
					}
					$tmp_comment .= "<p> ".$row->name."  ".$row->reviewed_time."<br />".stuolpad_outline_humen_view($row->comment)."</p>";
				}
				$output .="<span class='left'>Remarks </span><span class='right'>".$tmp_comment."</span>";
				
			$output .= "</div>";
			
			$output .= '<hr>';
			$output .= "<table class='upload-index'>";
			$output .= "<tr><th class='title'><h1 class='title'>Title</h1></th><th class='content'><h1 class='title'>Status</h1></th><th class='update'></th></tr>";
			// outline
			$output .= "<tr><td class='title'>Outline</td><td class='content'>".$outline."</td>";
			if(($user->uid == $content->tutorial_outline_uid && ($content->tutorial_outline_status != 4 && $content->tutorial_outline_status != 2 && $content->tutorial_outline_status != 7)) || $content->tutorial_outline_uid == 0 ){
				$output .= "<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/outline' method='post'><input type='hidden' name='outline' value='".$content->tutorial_outline."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='1'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			// script
			$output .= "<tr><td class='title'>Script</td><td class='content'>".$script."</td>";
			if(($user->uid == $content->tutorial_script_uid && ($content->tutorial_script_status != 4 && $content->tutorial_script_status != 2 && $content->tutorial_script_status != 7)) || $content->tutorial_script_uid == 0){
				$output .="<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/script' method='post'><input type='hidden' name='script' value='".$content->tutorial_script."'><input type='hidden' name='td_path' value='".$td_path."'><input type='hidden' name='language' value='".$content->language."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='1'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			// video
			$output .= "<tr><td class='title'>Video</td><td class='content'>".$video."</td>";
			if(($user->uid == $content->tutorial_video_uid && ($content->tutorial_video_status != 4 && $content->tutorial_video_status != 2 && $content->tutorial_video_status != 7)) || $content->tutorial_video_uid == 0){
				$output .= "<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/video' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='language' value='".$content->language."'><input type='hidden' name='td_path' value='".$td_path."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='1'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			$output .= "</table>";
			$output .= "</div>";

			return $output;

		}else{
			$output = "<p>Somthing went wrong. Please, try again</p>";
			return $output;
		}
	}
	
	function olang_update_index() {

		if($_REQUEST['tr']){

			global $user;
			$path = drupal_get_path('module', 'stupload');
			drupal_add_js($path . '/stupload.js');
			drupal_add_css($path . '/stupload.css');
			$trid = $_REQUEST['tr'];			

			$query = "select tr.tutorial_detail_id, tr.tutorial_content_id as tccid, tr.language, tr.tutorial_outline, tr.tutorial_outline_uid, tr.tutorial_outline_status, tr.tutorial_script, tr.tutorial_script_uid, tr.tutorial_script_status, tr.tutorial_script_timed, tr.tutorial_video, tr.tutorial_video_uid, tr.tutorial_video_status, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tcc.tutorial_slide, tcc.tutorial_slide_uid, tcc.tutorial_slide_status, tcc.tutorial_code, tcc.tutorial_code_uid, tcc.tutorial_code_status, tcc.tutorial_assignment, tcc.tutorial_assignment_uid, tcc.tutorial_assignment_status from tutorial_resources tr, tutorial_details td, tutorial_common_contents tcc where tr.id=".$trid." and td.id=tr.tutorial_detail_id and tcc.id=tr.tutorial_content_id";

			$result = db_query($query);
			$content = db_fetch_object($result);

			// tutirial status
			$outline = component_status($content->tutorial_outline_status, $content->tutorial_outline_uid, $content->tutorial_outline, 1);
			$script = component_status($content->tutorial_script_status, $content->tutorial_script_uid, $content->tutorial_script, 2);
			$video = component_status($content->tutorial_video_status, $content->tutorial_video_uid, $content->tutorial_video);
			
			$td_path = $content->foss_category.'/'.$content->tutorial_level.'/'.$content->tutorial_name.'/';
			
			$output = "<div class='stupload-form'>";
			$output .= "<div class='summery-title-content'>";
				$output .="<span class='left'>Foss Category</span><span class='right'>: ".$content->foss_category."</span>";
				$output .="<span class='left'>Tutorial Name</span><span class='right'>: ".$content->tutorial_name."</span>";
				$output .="<span class='left'>Level</span><span class='right'>: ".$content->tutorial_level."</span>";
				$output .="<span class='left'>Language</span><span class='right'>: ".$content->language."</span>";
				$output .="<span class='left'>Study Plan Code</span><span class='right'>: ".$content->order_code."</span>";
			$output .= "</div>";
			
			$output .= '<hr>';
			$output .= "<table class='upload-index'>";
			$output .= "<tr><th class='title'><h1 class='title'>Title</h1></th><th class='content'><h1 class='title'>Status</h1></th><th class='update'></th></tr>";
			// outline
			$output .= "<tr><td class='title'>Outline</td><td class='content'>".$outline."</td>";
			if($content->tutorial_outline_status != 4 || user_access('administer stupload')){
				$output .= "<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/outline' method='post'><input type='hidden' name='outline' value='".$content->tutorial_outline."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='2'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			// script
			$output .= "<tr><td class='title'>Script</td><td class='content'>".$script."</td>";
			if($content->tutorial_outline_status != 4 || user_access('administer stupload')){
				$output .="<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/script' method='post'><input type='hidden' name='script' value='".$content->tutorial_script."'><input type='hidden' name='td_path' value='".$td_path."'><input type='hidden' name='language' value='".$content->language."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='2'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			// video
			$output .= "<tr><td class='title'>Video</td><td class='content'>".$video."</td>";
			if(user_access('administer stupload')){
				$output .= "<td class='update'><form action='".$GLOBALS['base_url']."/stupload/update_regional_language_tutorial/video' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='language' value='".$content->language."'><input type='hidden' name='td_path' value='".$td_path."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='hidden' name='redirection' value='2'><input type='submit' value='Update'></form></td></tr>";
			}else{
				$output .= "<td><td></tr>";
			}
			
			$output .= "</table>";
			$output .= "</div>";

			return $output;

		}else{
			$output = "<p>Somthing went wrong. Please, try again</p>";
			return $output;
		}
	}
?>
