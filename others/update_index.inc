<?php

	function update_index($params) {
		global $user;
		if($params){
			$path = drupal_get_path('module', 'stupload');
			drupal_add_js($path . '/stupload.js');
			drupal_add_css($path . '/stupload.css');
			$params = parse_url($params);
			$tr_data = explode('=', $params['query']);

			$trid = $tr_data[1];
			
			$query = "select td.id as tdid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_script_timed, tr.tutorial_video, tcc.id as tccid, tcc.tutorial_slide, tcc.tutorial_code, tcc.tutorial_assignment from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$trid."'";

			$result = db_query($query);

			$content = db_fetch_object($result);

			// check if video exists ot not
			require_once('path.config.inc');

			if($content->tutorial_video != 'pending' && file_exists($file_upload_path.$content->tutorial_video)){
				//$video_url = $content->tutorial_video;
				$video_url = "<a href='".$tutorial_path.$content->tutorial_video."' target='_blank' >Accepted</a>";

			}else{
				$video_url = 'pending';
			}

			// check if slide exists ot not

			if($content->tutorial_slide != 'pending' && file_exists($file_upload_path.$content->tutorial_slide)){
				//$slide_url = $content->tutorial_slide;
				$slide_url = "<a href='".$tutorial_path.$content->tutorial_slide."' target='_blank'>Accepted</a>";
			}else{
				$slide_url = 'pending';
			}

			// check if codefile exists ot not

			if($content->tutorial_code != 'pending' && $content->tutorial_code != 'notrequired' && file_exists($file_upload_path.$content->tutorial_code)){
				//$codefile_url = $content->tutorial_code;
				$codefile_url = "<a href='".$tutorial_path.$content->tutorial_code."' target='_blank'>Accepted</a>";
			}elseif($content->tutorial_code == 'notrequired'){
				$codefile_url = 'notrequired';
			}else{
				$codefile_url = 'pending';
			}

			// check if assignment exists ot not
			if($content->tutorial_assignment != 'pending' && $content->tutorial_assignment != 'notrequired' && file_exists($file_upload_path.$content->tutorial_assignment)){
				//$codefile_url = $content->tutorial_assignment;
				$assignment_url = "<a href='".$tutorial_path.$content->tutorial_assignment."' target='_blank'>Accepted</a>";
			}elseif($content->tutorial_assignment == 'notrequired'){
				$assignment_url = 'notrequired';
			}else{
				$assignment_url = 'pending';
			}

			$output = "<div class='stupload-form'>";
			$output .= "<div class='summery-title-content'>";
				$output .="<span class='left'>Foss Category</span><span class='right'>: ".$content->foss_category."</span>";
				$output .="<span class='left'>Tutorial Name</span><span class='right'>: ".$content->tutorial_name."</span>";
				$output .="<span class='left'>Tutorial Level</span><span class='right'>: ".$content->tutorial_level."</span>";
				$output .="<span class='left'>Tutorial Language</span><span class='right'>: ".$content->language."</span>";
				//Previous Remarks for the tutorial
				$query = "select u.name, ci.reviewed_time, ci.comment from comment_improvement ci, users u where ci.trid='".$trid."' and u.uid=ci.uid order by ci.reviewed_time DESC";
				$result = db_query($query);
				if(!$result){
					$tmp_comment = ": - ";
				}
				while($row = db_fetch_object($result)){
					$tmp_comment .= "<p>".$row->name."  ".$row->reviewed_time."<br /><textarea readonly='readonly' rows='2' cols='50'>".$row->comment."</textarea></p>";
				}
				$output .="<span class='left'>Remarks </span><span class='right'>".$tmp_comment."</span>";
			$output .= "</div>";

			$output .= "<table class='upload-index'>";
			$output .= "<tr><th class='title'>Title</th><th class='content'>Status</th><th class='update'></th></tr>";
			
			$output .= "<tr><td class='title'>Study Plan</td><td class='content'>".$content->order_code."</td><td class='update'>";
				if($content->language == 'English' && ('admin_review_user' == in_array('admin_review_user', $user->roles) || 'admin_user' == in_array('admin_user', $user->roles))){
					$output .= "<form action='".$GLOBALS['base_url']."/stupload/update/studyplan' method='post'><input type='hidden' name='tdid' value='".$content->tdid."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";
				}else{
					$output .= "</td></tr>";
				}
			$output .= "<tr><td class='title'>Outline</td><td class='content'><textarea readonly='readonly' rows='6' cols='30'>".$content->tutorial_outline."</textarea></td><td class='update'><form action='".$GLOBALS['base_url']."/stupload/update/outline' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='outline' value='".$content->tutorial_outline."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";

			$output .= "<tr><td class='title'>Script</td><td class='content'>".$content->tutorial_script."</td><td class='update'><form action='".$GLOBALS['base_url']."/stupload/update/script' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";
			if($content->language == 'English')
				$output .= "<tr><td class='title'>Timed-script</td><td class='content'>".$content->tutorial_script_timed."</td><td class='update'><form action='".$GLOBALS['base_url']."/stupload/english_timed_script' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";

			$output .= "<tr><td class='title'>Video</td><td class='content'>".$video_url."</td><td class='update'><form action='".$GLOBALS['base_url']."/stupload/update/video' method='post'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";

			$output .= "<tr><td class='title'>Slide</td><td class='content'>".$slide_url."</td><td class='update'>";
				if($content->language == 'English'){
					$output .= "<form action='".$GLOBALS['base_url']."/stupload/update/slide' method='post'><input type='hidden' name='tccid' value='".$content->tccid."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";
				}else{
					$output .= "</td></tr>";
				}

			$output .= "<tr><td class='title'>Code files</td><td class='content'>".$codefile_url."</td><td class='update'>";
				if($content->language == 'English'){
					$output .= "<form action='".$GLOBALS['base_url']."/stupload/update/codefile' method='post'><input type='hidden' name='tccid' value='".$content->tccid."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";
				}else{
					$output .= "</td></tr>";
				}

			$output .= "<tr><td class='title'>Assignment</td><td class='content'>".$assignment_url."</td><td class='update'>";
				if($content->language == 'English'){
					$output .= "<form action='".$GLOBALS['base_url']."/stupload/update/assignment' method='post'><input type='hidden' name='tccid' value='".$content->tccid."'><input type='hidden' name='trid' value='".$trid."'><input type='hidden' name='tn' value='".$content->tutorial_name."'><input type='submit' value='Update'></form></td></tr>";
				}else{
					$output .= "</td></tr>";
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
