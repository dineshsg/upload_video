<?php
	function upload_english_script() {
		if($_REQUEST['trid'] && $_REQUEST['td_path'] && $_REQUEST['redirection']){

			$output = "<h2>Upload English Script for ".$_REQUEST['tn']."</h2>";
			$wiki_url = $_REQUEST['td_path'].'English';		
			$output .= "<div class='wiki-script-file'></div>";
			$output .= drupal_get_form('upload_english_script_form', $_REQUEST['trid'], $wiki_url, $_REQUEST['td_path'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}

	function upload_english_script_form($form_state, $trid, $wiki_url, $td_path, $redirection) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['wiki_url'] = array(
			'#type' => 'hidden',
			'#value' => $wiki_url,
			'#attributes' => array('class' => 'upeng_script_wiki'),
		);

		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		
		$form['td_path'] = array(
			'#type' => 'hidden',
			'#value' => $td_path,
		);
		
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		
		$form['script_status'] = array(
			'#type' => 'select',
			'#title' => t('Script'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_script_status'),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_script_form_validate($form, &$form_state){
	  	return;
	}
	
	function upload_english_script_form_submit($form, &$form_state){
		global $user;
		
		$tutorial_script = $form_state['values']['wiki_url'];
		$query = "UPDATE tutorial_resources SET tutorial_script_status = 1, tutorial_script_uid = $user->uid ,tutorial_script='".$tutorial_script."' WHERE id='".$form_state['values']['trid']."'";
		if($form_state['values']['script_status'] == 1){
			$query = "UPDATE tutorial_resources SET tutorial_script_status = 0, tutorial_script='pending', tutorial_script_uid = $user->uid WHERE id='".$form_state['values']['trid']."'";
		}
		db_query($query);
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'script')";

		if(db_query($query)){
			//Mail to contributor
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
			$result = db_query($query);
			$tutorial_detail = db_fetch_object($result);

			$a = new stdClass();
			$a->tc = 'Script';					
			$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);

		    $mailto = $user->mail;
		    $role = variable_get('contributor', '');
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			//Mail to domain and qc 
			$role = variable_get('quality_checker', '');
			$quality_mailid = get_language_role_emails($tutorial_detail->language);

			$role = variable_get('domain_user', '');
			$admin_mailid = get_role_emails($role);

			$mailto = $admin_mailid.$quality_mailid;

		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			drupal_set_message('Tutorial script Updated Successfully');
			$query= array('trid' => $form_state['values']['trid']);
			if($form_state['values']['redirection'] == 1){
				drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			} elseif($form_state['values']['redirection'] == 2){
				drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			}
		}else {
			drupal_set_message('Error while Adding Tutorial Script, Please try again.');
		}
	}
?>
