<?php
	function upload_english_codefile() {
		if($_REQUEST['trid'] && $_REQUEST['tccid'] && $_REQUEST['td_path'] && $_REQUEST['redirection']){
			$output = "<h2>Upload English Codefile for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_codefile_form', $_REQUEST['trid'], $_REQUEST['tccid'], $_REQUEST['td_path'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}

	function upload_english_codefile_form($form_state, $trid, $tccid, $td_path, $redirection) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['tccid'] = array(
			'#type' => 'hidden',
			'#value' => $tccid,
		);
		$form['td_path'] = array(
			'#type' => 'hidden',
			'#value' => $td_path,
		);
		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		$form['codefile_status'] = array(
			'#type' => 'select',
			'#title' => t('Codefile Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Will upload later', 2 => 'Not Required'),
			'#attributes' => array('class' => 'upeng_codefile_status'),
		);
		$form['codefile'] = array(
			'#type' => 'file',
			'#title' => t('Codefile <span class="upload-help-test"> (Upload .zip file only)</span>'),
			'#attributes' => array('class' => 'upeng_codefile'),
			'#prefix' => '<div class="stupload-form-codefile">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_codefile_form_validate($form, &$form_state){
		if ($form_state['values']['codefile_status'] == 0){
			if($_FILES['files']['tmp_name']['codefile'] == ''){
				form_set_error('codefile', t('Please select the codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.')); // get file name
				$filesize = $_FILES['files']['size']['codefile'];
				$allowed_file_types = explode(',', variable_get('codefile_extensions', ''));
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('codefile', t('Please select proper file. .Zip file only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_codefile_form_submit($form, &$form_state){
		global $user;
		$td = explode('/', $_POST['td_path']);	
		$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.'));
		
		$file_name = $td[2].'-Codefile-'.English.$file_ext;
		$tutorial_codefile = 'st_videos/'.$_POST['td_path'].'Resource/'.$file_name;
		$path = variable_get('file_upload_path', '').$tutorial_codefile;
		// for upload later
		if ($form_state['values']['codefile_status'] == 1 || $form_state['values']['codefile_status'] == 2){
			global $user;
			$tutorial_codefile = (($form_state['values']['codefile_status'] == 1) ? 'pending' : 'notrequired');
			$tutorial_codefile_status = (($form_state['values']['codefile_status'] == 1) ? 0 : 6);
			// insert into common data table
			$query = "UPDATE tutorial_common_contents SET tutorial_code_uid=".$user->uid.", tutorial_code_status=".$tutorial_codefile_status.", tutorial_code='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
			db_query($query);

			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Codefile Added Successfully');
				$query= array('trid' => $form_state['values']['trid']);
				drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			}else {
				drupal_set_message('Error while Adding Codefile, Please try again.');
			}			
		}
		// for upload	
		if($_FILES['files']['size']['codefile'] > 0){

			if(move_uploaded_file($_FILES['files']['tmp_name']['codefile'], $path)){
				global $user;
				// insert into common data table
				
				$query = "UPDATE tutorial_common_contents SET tutorial_code_uid=".$user->uid.", tutorial_code_status=2, tutorial_code='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->tc = 'Codefile';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = $user->mail;
				    $role = variable_get('contributor', '');
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user 

					$role = variable_get('quality_checker', '');
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					
					$role = variable_get('domain_userr', '');
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;

				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					drupal_set_message('Tutorial Codefile Added Successfully');
					$query= array('trid' => $form_state['values']['trid']);
					if($form_state['values']['redirection'] == 1){
						drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					} elseif($form_state['values']['redirection'] == 2){
						drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					}
				}else {
					drupal_set_message('Error while Adding Codefile, Please try again.');
				}
			}else{
				drupal_set_message('Error while Adding Codefile, Please try again..');
			}
		}
}

?>
