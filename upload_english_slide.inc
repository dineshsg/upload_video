<?php
	function upload_english_slide() {
		if($_REQUEST['trid'] && $_REQUEST['tccid'] && $_REQUEST['td_path'] && $_REQUEST['redirection']){		
			$output = "<h2>Upload English Slide for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_slide_form', $_REQUEST['trid'], $_REQUEST['tccid'], $_REQUEST['td_path'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}

	}

	function upload_english_slide_form($form_state, $trid, $tccid, $td_path, $redirection) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		$form['tccid'] = array(
			'#type' => 'hidden',
			'#value' => $tccid,
		);
		$form['td_path'] = array(
			'#type' => 'hidden',
			'#value' => $td_path,
		);
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		$form['slide_status'] = array(
			'#type' => 'select',
			'#title' => t('Slide Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_slide_status'),
		);
		$form['slide'] = array(
			'#type' => 'file',
			'#title' => t('Slide <span class="upload-help-test"> (Upload .zip file only)</span>'),
			'#attributes' => array('class' => 'upeng_slide'),
			'#prefix' => '<div class="stupload-form-slide">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
		return $form;
	
	}
	
	
	function upload_english_slide_form_validate($form, &$form_state){
		if ($form_state['values']['slide_status'] == 0){
			if($_FILES['files']['tmp_name']['slide'] == ''){
				form_set_error('slide', t('Please select the slide file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['slide'], strripos($_FILES['files']['name']['slide'], '.')); // get file name
				$filesize = $_FILES['files']['size']['slide'];
				var_dump(variable_get('slide', ''));
				$allowed_file_types = explode(',', variable_get('slide_extensions', ''));
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('slide', t('Please select proper file. zip file only allowed.'));
				}
			}
		}
	  	return;
	}
	
	function upload_english_slide_form_submit($form, &$form_state){
		global $user;
		$td = explode('/', $_POST['td_path']);	
		$file_ext = substr($_FILES['files']['name']['slide'], strripos($_FILES['files']['name']['slide'], '.'));
		
		$file_name = $td[2].'-Slide-'.English.$file_ext;
		$tutorial_slide = 'st_videos/'.$_POST['td_path'].'Resource/'.$file_name;
		$path = variable_get('file_upload_path', '').$tutorial_slide;
		
		// for upload later
		if ($form_state['values']['slide_status'] == 1){
			$tutorial_slide = 'pending';
			// insert into common data table
			$query = "UPDATE tutorial_common_contents SET tutorial_slide_status=0, tutorial_slide_uid=".$user->uid.", tutorial_slide='pending' WHERE id='".$form_state['values']['tccid']."'";
			db_query($query);

			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'slide')";

			if(db_query($query)){
				drupal_set_message('Tutorial Slide Added Successfully');
				$query= array('trid' => $form_state['values']['trid']);
				if($form_state['values']['redirection'] == 1){
					drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
				} elseif($form_state['values']['redirection'] == 2){
					drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
				}
			}else {
				drupal_set_message('Error while Adding Slide, Please try again.');
			}			
		}
		// for upload file
		if($_FILES['files']['size']['slide'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['slide'], $path)){
				global $user;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_slide_uid=".$user->uid.", tutorial_slide_status='1', tutorial_slide='".$tutorial_slide."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);
				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'Slide')";
				if(db_query($query)){
					//Mail to contributor
					$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
					$result = db_query($query);
					$tutorial_detail = db_fetch_object($result);
					$a = new stdClass();
					$a->tc = 'Slide';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = $user->mail;
				    $role = variable_get('contributor', '');
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user 
					$role = variable_get('quality_checker', '');
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					
					$role = variable_get('domain_user', '');
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;

				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					drupal_set_message('Tutorial Slide Added Successfully');
					$query= array('trid' => $form_state['values']['trid']);
					if($form_state['values']['redirection'] == 1){
						drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					} elseif($form_state['values']['redirection'] == 2){
						drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					}
				}else {
					drupal_set_message('Error while Adding Slide, Please try again.');
				}
			}else{
				drupal_set_message('Error while Slide, Please try again..');
			}
		}
	}
?>
