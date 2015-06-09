<?php
	function update_outline() {
		if($_REQUEST){
			$output = "<h2>Update Outline for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('update_outline_form', $_REQUEST['trid'], $_REQUEST['outline']);
			return $output;
		}else{
			$output = "Somthing went wrong. Please try again";
			return $output;
		}
		
	}
	function update_outline_form($form_state, $trid, $outline) {
		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addtutorial-name');

		$form['trid'] = array(
			'#type' => 'hidden',
			'#title' => t('id'),
			'#required' => TRUE,
			'#value' => $trid
		);
		$form['outline'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#value' => $outline,
			'#attributes' => array('class' => 'uptn_outline'),
			'#prefix' => '<div class="stupload-outlineu">',
			'#suffix' => '</div>'
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function update_outline_form_validate($form, &$form_state){
	
		if((strlen($form_state['values']['outline']) < 10)){
			form_set_error('outline', t('Please, enter atleast 10 Characters'));
		}
		
	  	return;
	}
	
	function update_outline_form_submit($form, &$form_state){
		global $user;

		//$tmp_out = mysql_real_escape_string($form_state['values']['outline']);
		//var_dump($form_state['values']['outline']);
		//exit;
		$query = "UPDATE tutorial_resources SET tutorial_status='pending', tutorial_outline='".db_escape_string($form_state['values']['outline'])."' WHERE id='".$form_state['values']['trid']."'";

		db_query($query);

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'outline')";

		if(db_query($query)){
			
			//Mail to contributor
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
			$result = db_query($query);
			$tutorial_detail = db_fetch_object($result);

			$a = new stdClass();
			$a->tc = 'Outline';					
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

			drupal_set_message('Tutorial Outline Updated successfully.');
			$redirect = 'stupload/update_index/?tr='.$form_state['values']['trid'];		

			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while update Outline, Please try again.');
		}	

}


?>
