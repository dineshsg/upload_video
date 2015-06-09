<?php
	function review_tutorial($params) {

		$data = explode('=', $params);
		$trid = $data[1];
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		drupal_add_js($path . '/video.js');
		drupal_add_css($path . '/video-js.css');
		global $user;
		if($trid ){
			$query = "select td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.id as trid, tr.language, tr.tutorial_status, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_video, tcc.tutorial_slide, tcc.tutorial_code, tcc.tutorial_assignment from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$trid."'";
			$result = db_query($query);
			$tutorial = db_fetch_object($result);

			require_once('path.config.inc');

			//for codefile
			if($tutorial->tutorial_code != 'pending' && $tutorial->tutorial_code != 'notrequired' && file_exists($file_upload_path.$tutorial->tutorial_code)){
				$codefile = "<ul><li><a href='".$tutorial_path.$tutorial->tutorial_code."'>Click here to view code files</a></li></ul>";
				$codefile .= read_ziparchive($file_upload_path.$tutorial->tutorial_code);
			}else{
				$codefile = $tutorial->tutorial_code;
			}

			// for assignment

			if($tutorial->tutorial_assignment != 'pending' && $tutorial->tutorial_assignment != 'notrequired' && file_exists($file_upload_path.$tutorial->tutorial_assignment)){
				$file_ext = substr($tutorial->tutorial_assignment, strripos($tutorial->tutorial_assignment, '.'));
				$file_name = str_replace('/', '', substr($tutorial->tutorial_assignment, strripos($tutorial->tutorial_assignment, '/')));
				if($file_ext == '.txt' || $file_ext == '.pdf'){
					$assign = "<a href='".$tutorial_path.$tutorial->tutorial_assignment."' target='_blank'>".$file_name."</a>";
				}
			}else{
				$assign = $tutorial->tutorial_assignment;
			}

			// for slide
			if($tutorial->tutorial_slide != 'pending' && file_exists($file_upload_path.$tutorial->tutorial_slide)){
				$slide = "<ul><li><a href='".$tutorial_path.$tutorial->tutorial_slide."'>Click here to view slides</a></li></ul>";
				// $slide .= read_ziparchive($file_upload_path.$tutorial->tutorial_slide);
			}else{
				$slide = $tutorial->tutorial_slide;
			}

			// video
			if($tutorial->tutorial_video != 'pending' && file_exists($file_upload_path.$tutorial->tutorial_video)){
				$file_ext = substr($tutorial->tutorial_video, strripos($tutorial->tutorial_video, '.'));
				$movie = new ffmpeg_movie($file_upload_path.$tutorial->tutorial_video);
				$height = $movie->getFrameHeight();
				$width = $movie->getFrameWidth();
				$time = gmdate("H:i:s", $movie->getDuration());
				$bitrate = $movie->getBitRate();
				$fps = $movie->getFrameRate();
				$size = filesize($file_upload_path.$tutorial->tutorial_video);
				$video_info = '<p><table border"0" cellpadding="4" style="width: 100%; border-collapse: inherit;"><tr><td><b>File Size:</b> '._format_bytes($size).'</td>';
				$video_info .= '<td><b>Video Timing: </b>'.$time.'</td></tr>';
				$video_info .= '<tr><td><b>Frame Width:</b> '.$width.'</td>';
				$video_info .= '<td><b>Frame Height:</b> '.$height.'</td></tr>';
				$video_info .= '<tr><td><b>Frame Rate(fps):</b> '.$fps.'</td>';
				$video_info .= '<td><b>Bit Rate:</b> '.$bitrate.'/s</td></tr></table></p>';
				$video ='<video id="example_video_1" class="video-js vjs-default-skin" controls width="400" height="300" data-setup="{}">';
		    if($file_ext == '.mp4'){
                    	$video .='<source src="'.$tutorial_path.$tutorial->tutorial_video.'" type="video/mp4" />';
                    }else{
                    	$video .='<source src="'.$tutorial_path.$tutorial->tutorial_video.'" type="video/ogg" />';
                    }
                    $video .= '</video>';
			}else{
				$video = $tutorial->tutorial_video;
			}
			// script
			if($tutorial->tutorial_script != 'pending'){
				$script = "<iframe width='100%' height='600px' src='".$wiki_path.$tutorial->tutorial_script."'></iframe>";
			}else{
				$script = $tutorial->tutorial_script;
			}

			// show current status
			$current_status = array('pending' => 'Pending', 'under_review' => 'Under Review', 'need_improvement' => 'Need Improvement', 'accepted' => 'Accepted', 'publish' => 'Published');


			$output = "<div class='stupload-form'>";
				$output .= "<div class='summery'>";
					$output .= "<div class='summery-title-content'>";
						$output .="<span class='left'>Foss Category</span><span class='right'>: ".$tutorial->foss_category."</span>";
						$output .="<span class='left'>Name</span><span class='right'>: ".$tutorial->tutorial_name."</span>";
						$output .="<span class='left'>Level</span><span class='right'>: ".$tutorial->tutorial_level."</span>";
						$output .="<span class='left'>Language</span><span class='right'>: ".$tutorial->language."</span>";
						$output .="<span class='left'>Current Status</span><span class='right'>: ".$current_status[$tutorial->tutorial_status]."</span>";
						// set status
						if($tutorial->tutorial_status == 'pending'){
							$status = '';
						}else{
							$status = $tutorial->tutorial_status;
						}
						// if need improvement, form should be hide
						if($tutorial->tutorial_status != 'need_improvement'){
							$output .="<span class='left'></span><span class='right'>".drupal_get_form('review_tutorial_form', $trid, $status)."</span>";
						}
						//Previous Remarks for the tutorial
						$query = "select u.name, ci.reviewed_time, ci.comment from comment_improvement ci, users u where ci.trid='".$trid."' and u.uid=ci.uid order by ci.reviewed_time DESC";
						$result = db_query($query);
						$tmp_comment = " - ";
						while($row = db_fetch_object($result)){
							if($tmp_comment == " - "){
								$tmp_comment = '';
							}
							$tmp_comment .= "<p> ".$row->name."  ".$row->reviewed_time."<br /><textarea readonly='readonly' rows='2' cols='48'>".$row->comment."</textarea></p>";
						}
						$output .="<span class='left'>Remarks </span><span class='right'>".$tmp_comment."</span>";
					$output .= "</div>";
					$output .= "<div class='left-content'>";
						$output .= "<div class='summery-outline'>";
							$output .= "<p class='summery-title'>Outline</p>";
							$output .= "<p class='summery-content'><textarea rows='6' cols='30' readonly=readonly>".$tutorial->tutorial_outline."</textarea></p>";
						$output .= "</div>";
						$output .= "<div class='summery-slide'>";
							$output .= "<p class='summery-title'>Slide</p>";
							$output .= "<p class='summery-content'>".$slide."</p>";
						$output .= "</div>";
						$output .= "<div class='summery-codefile'>";
							$output .= "<p class='summery-title'>Codefile</p>";
							$output .= "<p class='summery-content'>".$codefile."</p>";
						$output .= "</div>";
						$output .= "<div class='summery-assignment'>";
							$output .= "<p class='summery-title'>Assignment</p>";
							$output .= "<p class='summery-content'>".$assign."</p>";
						$output .= "</div>";
					$output .= "</div>";
					$output .= "<div class='right-content'>";
						$output .= "<div class='summery-video'>";
							$output .= "<p class='summery-title'>Video</p>";
							$output .= $video_info;
							$output .= "<p class='summery-content'>".$video."</p>";
						$output .= "</div>";
					$output .= "</div>";
					$output .= "<div class='summery-script'>";
						$output .= "<p class='summery-title'>Script</p>";
						$output .= "<p class='summery-content'>".$script."</p>";
					$output .= "</div>";
				$output .= "</div>";
			$output .= "</div>";
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}


	}

	function _format_bytes($a_bytes)
	{
	    if ($a_bytes < 1000) {
	        return $a_bytes .' B';
	    } elseif ($a_bytes < 1048576) {
	        return round($a_bytes / 1000, 2) .' KB';
	    } elseif ($a_bytes < 1073741824) {
	        return round($a_bytes / 1000000, 2) . ' MB';
	    }
	}

	function review_tutorial_form($form_state, $trid, $status){
		// add js file 
		global $user;
		if(in_array('domain_user', $user->roles)){
			// check if tutorial pending or not
			$query = "select tr.tutorial_outline, tr.tutorial_script, tr.tutorial_script_timed, tr.tutorial_video, tcc.tutorial_slide, tcc.tutorial_code, tcc.tutorial_assignment from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$trid."'";

			$result = db_query($query);

			$row = db_fetch_array($result);
			$row['tutorial_script_timed'] = 'notnecessary';
			$options = array('pending' => 'Accepted', 'need_improvement' => 'Need Improvement');
			if(!in_array('pending', $row)){
				$options = array('reviewed' => 'Accept the tutorial', 'need_improvement' => 'Need improvement');
			}
		}else{
			$options = array('under_review' => 'Under Review', 'need_improvement' => 'Need Improvement');
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

		$form['status'] = array(
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
	function review_tutorial_form_submit($form, &$form_state){

		global $user;

		// Get the Tutorial Details
		$query = "select tr.language, tr.uid, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
		$result = db_query($query);
		$tutorial_detail = db_fetch_object($result);

		$a = new stdClass();
		$a->tc = 'Review';					
		$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
		// send Email
		if(in_array('domain_user', $user->roles)){
			// Mail to Contibutor
			if($form_state['values']['status'] == 'pending' || $form_state['values']['status'] == 'need_improvement'){
				//Mail to contributor
			    $role = 'contributor';
			    $mailto = get_contributor_email($tds->uid);
			    $message = (($form_state['values']['status'] == 'need_improvement') ? $form_state['values']['comment'] : '');
				stupload_notify($mailto, $role, $message, $tds, $user->name);

			}else if($form_state['values']['status'] == 'accepted'){  // accept overall tutorial 

				//Mail to contributor
			    $role = 'contributor';
			    $mailto = get_contributor_email($tds->uid);
			    $message = $form_state['values']['comment'];
				stupload_notify($mailto, $role, $message, $tds, $user->name);

				//Mail to Admin User 
				$role = 'admin_user';
				$mailto = get_role_emails($role);
			    $message = "";
				stupload_notify($mailto, $role, $message, $tds, $user->name);

			}

		}else if(in_array('admin_review_user', $user->roles)){
			// Mail to Contibutor and Domain Reviewr(domain_user)
			if($form_state['values']['status'] == 'under_review'){
				//Mail to contributor
			    $role = 'contributor';
			    $mailto = get_contributor_email($td['uid']);
			    $message = "";
				stupload_notify($mailto, $role, $message, $tds, $user->name);

				//Mail to domain_user Domail reviewer 
				$role = 'domain_user';
				$mailto = get_role_emails($role);
			    $message = "";
				stupload_notify($mailto, $role, $message, $tds, $user->name);
			}else{  // need_improvement

				//Mail to contributor
			    $role = 'contributor';
			    $mailto = get_contributor_email($td['uid']);
			    $message = $form_state['values']['comment'];
				stupload_notify($mailto, $role, $message, $tds, $user->name);
			}
		}
		
		$query = "UPDATE tutorial_resources SET reviewer='".$user->name."', tutorial_status='".$form_state['values']['status']."' WHERE id='".$form_state['values']['trid']."'";
		db_query($query);

		// insert into the comment improvement table
		if($form_state['values']['comment'] != ''){
			$query = "INSERT INTO `comment_improvement` (`id`, `uid`, `trid`, `reviewed_time`, `comment`) VALUES (NULL, '".$user->uid."', '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$form_state['values']['comment']."')";

			db_query($query);
		}

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'review')";

		if(db_query($query)){
			drupal_set_message('Tutorial Status Updated successfully.');
			$redirect = 'stupload/upload/review/?tr='.$form_state['values']['trid'];
			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
	}
?>
