<?php

function request_exception_form($form_state,$arg1)
{
		$info_array1 = array();
		$info_array2 = array();
		$argu1 = unserialize(base64_decode($arg1));
		$argu1_encoded=htmlentities($arg1);
		$minFrame = 2;
		$maxFrame = 4;
		$duration=$argu1[0];
		$frameRate=$argu1[1];
		$height=$argu1[2];
		$width=$argu1[3];
		$bitRate=$argu1[4];
		$td_path=$argu1[6];
		$trid=$argu1[7];
		$move_url=$argu1[8];
		$file_ext1=$argu1[10];
		//$video_size1=(($argu1[9])/1024)/1024;
		//$video_size=round($video_size1,2);
		$time = $argu1[0];
		$video_size = $argu1[9];
		$form = array();
		drupal_add_js(drupal_get_path('module', 'stupload') . '/js/stupload.js');
		$form['td_path'] = array(
			'#type' => 'hidden',
			'#value' => $td_path,
		);
		$form['file_det'] = array(
			'#type' => 'hidden',
			'#value' => $file_det,
		);
		$form['video_size'] = array(
			'#type' => 'hidden',
			'#value' => $video_size,
		);
		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		$form['file_ext'] = array(
			'#type' => 'hidden',
			'#value' => $file_ext1,
		);
		$form['move_url'] = array(
			'#type' => 'hidden',
			'#value' => $move_url,
		);
		$form['header'] = array(
			'#type' => 'item',
			'#value' => '<th><b>Error in the Parameter</b></th><th><b>Required</b></th><th><b>Uploaded Video</b></th>',
			'#prefix' => '<table border="1"><tr>',
			'#suffix' => '</tr>',

		);
		if($height<600)
		{
		$form['message1'] = array(
			'#type' => 'item',
			'#value' => '<td>Height</td><td>600</td><td><b style="color:red;">'.$height.'</b></td>',
			'#prefix' => '<tr>',
			'#suffix' => '</tr>',

		);
		}
		if($width<800)
		{
			$form['message2'] = array(
			'#type' => 'item',
			'#value' => '<td>Width</td><td>800</td><td><b style="color:red;">'.$width.'</b></td>',
			'#prefix' => '<tr>',
			'#suffix' => '</tr>',
			);
		}
		if(($frameRate < $minFrame)||($frameRate>$maxFrame))
		{
			if($frameRate < $minFrame)
			{
				$form['message3'] = array(
					'#type' => 'item',
					'#value' => '<td>Minimum FrameRate</td><td>'.$minFrame.'</td><td><b style="color:red;">'.$frameRate.'</b></td>',
					'#prefix' => '<tr>',
					'#suffix' => '</tr>',
				);
			}
			else
			{
				$form['message4'] = array(
					'#type' => 'item',
					'#value' => '<td>Maximum Frame Rate</td><td>'.$maxFrame.'</td><td><b style="color:red;">'.$frameRate.'</b></td>',
					'#prefix' => '<tr>',
					'#suffix' => '</tr>',
				);
			}
		}
		$time_ac = gmdate("i:s",$time);
		

		if($time < 480)
		{
			$form['message5'] = array(
					'#type' => 'item',
					'#value' => '<td>Minimum Duration</td><td>8 minutes</td><td><b style="color:red;">'.$time_ac.'</b></td>',
					'#prefix' => '<tr>',
					'#suffix' => '</tr>',
				);
		}
		elseif($time > 720)
		{
			$form['message6'] = array(
					'#type' => 'item',
					'#value' => '<td>Maximum Duration</td><td>12 minutes</td><td><b style="color:red;">'.$time_ac.' minutes</b></td>',
					'#prefix' => '<tr>',
					'#suffix' => '</tr>',
				);
		}
		if($video_size > 15)
		{
			$form['message5'] = array(
					'#type' => 'item',
					'#value' => '<td>Minimum File Size</td><td>15 MB</td><td><b style="color:red;">'.$video_size.' MB</b></td>',
					'#prefix' => '<tr>',
					'#suffix' => '</tr>',
				);
		}
		$form['message7'] = array(
					'#type' => 'item',
					'#value' => '</table>',
				);
		$form['error_msg'] = array(
			'#type' => 'textarea',
			'#title' => 'Request A Exception',
			'#required' => TRUE,
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
	    );
		$form['cancel'] = array(
			'#value' => 'Close',
			'#type' => 'button',
			'#attributes' => array('onClick' => "parent.Lightbox.end('forceClose'); return false;"),
		);
		return $form;
}

function request_exception_form_submit($form, &$form_state)
{
	
	global $user;
	
		$td = explode('/', $_POST['td_path']);
		$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.'));
		$file_name = $td[2].'-English'.$form_state['values']['file_ext'];
		
		$tutorial_video = 'st_videos/'.$_POST['td_path'].$file_name;
		$path = variable_get('file_upload_path', '').$tutorial_video;
		
		// upload later
		
			//$query = "UPDATE tutorial_resources SET tutorial_video_uid=".$user->uid.",request_exception='".$_POST['error_msg']."',tutorial_video_status=8, cvideo_version=1, tutorial_video='pending' WHERE id='".$form_state['values']['trid']."'";
			
		$query = "UPDATE tutorial_resources SET request_exception='".$_POST['error_msg']."',tutorial_video_status=8 WHERE id='".$form_state['values']['trid']."'";
			db_query($query);
			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Video Updated Successfully');
				$query= array('trid' => $form_state['values']['trid']);
				
			}else {
				drupal_set_message('Error while Adding Tutorial Video, Please try again.');
			}
		
		
		if($form_state['values']['video_size'] > 0){
			
				// insert into common data table
				//$query = "UPDATE tutorial_resources SET tutorial_video_uid=".$user->uid.", tutorial_video_status=8, cvideo_version=1, tutorial_video='".$tutorial_video."' WHERE id='".$form_state['values']['trid']."'";
				$query = "UPDATE tutorial_resources SET request_exception='".$_POST['error_msg']."',tutorial_video_status=8 WHERE id='".$form_state['values']['trid']."'";
				db_query($query);
				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";
				db_query($query);
				$query = "select td.foss_category, td.tutorial_name, td.tutorial_level , tr.tutorial_video_status, tr.language, tr.tutorial_outline_uid as Outline, tr.tutorial_script_uid as Script, tr.tutorial_video_uid as Video, tcc.tutorial_slide_uid as Slide, tcc.tutorial_code_uid as Codefile, tcc.tutorial_assignment_uid as Assignment from tutorial_resources tr,  tutorial_details td, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.id=".$_POST['trid']." and tcc.id=tutorial_content_id";
		$result = db_query($query);
		
		$tutorial_detail = db_fetch_object($result);
				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->stage = "expreview";
					$a->tc = "Video";
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
					$mailto = $user->mail;
					$role = variable_get('contributor', '');
					$message = $_POST['error_msg'];
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user
					$role = variable_get('quality_checker', '');
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					$mailto = $quality_mailid;
					//var_dump($quality_mailid);
					//die;
					//$message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);
					drupal_set_message('Tutorial Video Updated Successfully');
					$query= array('trid' => $form_state['values']['trid']);
					
				}else {
					drupal_set_message('Error while Adding Tutorial Video, Please try again.');
				}
		if($tutorial_detail->language == 'English')
				{
			die('<script type="text/javascript"> parent.window.location="http://www.spoken-tutorial.org/stupload/upload_english_tutorial_index?trid='.$form_state['values']['trid'].'"; parent.Lightbox.end("forceClose");</script>');
				}
				else
				{
			die('<script type="text/javascript"> parent.window.location="http://www.spoken-tutorial.org/stupload/upload_regional_language_tutorial_index?tr='.$form_state['values']['trid'].'"; parent.Lightbox.end("forceClose");</script>');
				}
		}
			
		
}
?>
