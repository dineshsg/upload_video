<?php
	function upload_english_assignment() {
		if($_REQUEST['trid'] && $_REQUEST['tccid'] && $_REQUEST['td_path'] && $_REQUEST['redirection']){		
			$output = "<h2>Upload English Assignment for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_assignment_form', $_REQUEST['trid'], $_REQUEST['tccid'], $_REQUEST['td_path'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}
	function upload_english_assignment_form($form_state, $trid, $tccid, $td_path, $redirection) {
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
		$form['asgmnt_status'] = array(
			'#type' => 'select',
			'#title' => t('Assignment Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Will upload later', 2 => 'Not Required'),
			'#attributes' => array('class' => 'upeng_asgmnt_status'),
		);
		$form['assignment'] = array(
			'#type' => 'file',
			'#title' => t('Assignment <span class="upload-help-test"> (Upload .txt and .pdf files only)</span>'),
			'#attributes' => array('class' => 'upeng_asgmnt'),
			'#prefix' => '<div class="stupload-form-asgmnt">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_assignment_form_validate($form, &$form_state){
		if ($form_state['values']['asgmnt_status'] == 0){
			if($_FILES['files']['tmp_name']['assignment'] == ''){
				form_set_error('codefile', t('Please select the codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['assignment'], strripos($_FILES['files']['name']['assignment'], '.')); // get file name
				$filesize = $_FILES['files']['size']['assignment'];
				$allowed_file_types = explode(',', variable_get('assignment_extensions', ''));
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('assignment', t('Please select proper file. txt and pdf files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_assignment_form_submit($form, &$form_state){

		global $user;
		$td = explode('/', $_POST['td_path']);	
		$file_ext = substr($_FILES['files']['name']['assignment'], strripos($_FILES['files']['name']['assignment'], '.'));
		$file_name = $td[2].'-Assignment-'.English.$file_ext;
		$tutorial_assignment = 'st_videos/'.$_POST['td_path'].'Resource/'.$file_name;
		$path = variable_get('file_upload_path', '').$tutorial_assignment;
		
		// for upload later and not required
		if ($form_state['values']['asgmnt_status'] == 1 || $form_state['values']['asgmnt_status'] == 2){
			global $user;
			$tutorial_assign = (($form_state['values']['asgmnt_status'] == 1) ? 'pending' : 'notrequired');
			$tutorial_assign_status = (($form_state['values']['asgmnt_status'] == 1) ? 0 : 6);
			// insert into common data table
			$query = "UPDATE tutorial_common_contents SET tutorial_assignment_uid=".$user->uid.", tutorial_assignment_status=".$tutorial_assign_status.", tutorial_assignment='".$tutorial_assign."' WHERE id='".$form_state['values']['tccid']."'";

			db_query($query);

			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Codefile Adding Successfully');
				$query= array('trid' => $form_state['values']['trid']);
				drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			}else {
				drupal_set_message('Error while Adding Assignment, Please try again.');
			}
		}
		// for upload assignment
		if($_FILES['files']['size']['assignment'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['assignment'], $path)){
				global $user;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_assignment_uid=".$user->uid.", tutorial_assignment_status=2, tutorial_assignment='".$tutorial_assignment."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->tc = 'Assignment';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = $user->mail;
				    $role = variable_get('contributor', '');
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user 
					$role = variable_get('quality_checker', '');
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					
					$role = variable_get('domail_user', '');
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;

				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					drupal_set_message('Tutorial Assignment Adding Successfully');
					$query= array('trid' => $form_state['values']['trid']);
					if($form_state['values']['redirection'] == 1){
						drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					} elseif($form_state['values']['redirection'] == 2){
						drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					}
				}else {
					drupal_set_message('Error while Adding Assignment, Please try again.');
				}
			}else{
				drupal_set_message('Error while Adding Assignment, Please try again..');
			}
		}

	}

?>
