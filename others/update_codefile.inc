<?php
	function update_codefile() {
		if($_POST){
			$output = "<h2>Update Codefile for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('update_codefile_form', $_POST['trid'], $_POST['tccid']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}

	}


	function update_codefile_form($form_state, $trid, $tccid) {
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
	
	function update_codefile_form_validate($form, &$form_state){
		if ($form_state['values']['codefile_status'] == 0){
			if($_FILES['files']['tmp_name']['codefile'] == ''){
				form_set_error('slide', t('Please select the Codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.')); // get file name
				$filesize = $_FILES['files']['size']['codefile'];
				$allowed_file_types = array('.zip');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('codefile', t('Please select proper file. zip files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function update_codefile_form_submit($form, &$form_state){

		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";

		$tdresult = db_query($query);
		$tutorial_detail = db_fetch_object($tdresult);

		$codefile_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/Resource/';
		require_once('path.config.inc');
		$path = $file_upload_path.$codefile_path;
		$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-Codefile-'.$tutorial_detail->language.$file_ext;
		// for upload later
		if ($form_state['values']['codefile_status'] == 1 || $form_state['values']['codefile_status'] == 2){
			global $user;
			$tutorial_codefile = (($form_state['values']['codefile_status'] == 1) ? 'pending' : 'notrequired');
			// insert into common data table
			$query = "UPDATE tutorial_common_contents SET tutorial_code='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
			db_query($query);

			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Codefile Added Successfully');
				$redirect = 'stupload/english_index/?tr='.$form_state['values']['trid'];
				drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
			}else {
				drupal_set_message('Error while Adding Codefile, Please try again.');
			}			
		}
		// for upload	
		if($_FILES['files']['size']['codefile'] > 0){
			$bac_file_name = 'backup_'.$tutorial_detail->tutorial_name.'-'.$tutorial_detail->language.$file_ext;
			if(file_exists($path.$file_name) && !rename($path.$file_name, $path.$bac_file_name)){
				drupal_set_message('Unable to get backup file');
			}
			if(move_uploaded_file($_FILES['files']['tmp_name']['codefile'], $path.$file_name)){

				// delete backup file
				if(file_exists($path.$bac_file_name) && !unlink($path.$bac_file_name)){
					drupal_set_message('Unable to delete backup file');
				}
				global $user;

				$tutorial_codefile = $codefile_path.$file_name;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_code='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->tc = 'Codefile';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = $user->mail;
				    $role = 'contributor';
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user 
					$role = 'quality_checker';
					$quality_mailid = get_language_role_emails($tutorial_detail->language);

					$role = 'admin_review_user';
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);
					
					drupal_set_message('Tutorial Codefile Updated Successfully');
					$redirect = 'stupload/update_index/?tr='.$form_state['values']['trid'];
					drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					if(file_exists($path.$bac_file_name)){
						rename($path.$bac_file_name, $path.$file_name);
					}
					drupal_set_message('Error while Updating Codefile, Please try again.');
				}
			}else{
				drupal_set_message('Error while Updating Codefile, Please try again..');
			}
		}

}

?>
