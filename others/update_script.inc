<?php
	function update_scripts() {

		if($_POST){
			
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$_POST['trid']."'";

			$result = db_query($query);
			$tutorial_detail = db_fetch_object($result);
			require_once('path.config.inc');
			$wiki_url = $tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/'.$tutorial_detail->language;
			$output = "<h2>Update Script for ".$_REQUEST['tn']."</h2><br /><b>Script:</b>";
			$output .= "<div class='wiki-script-file'><iframe width='100%' height='600px' src='".$wiki_path.$wiki_url."'></iframe></div>";
			$output .= drupal_get_form('update_script_form', $_POST['trid'], $wiki_url);

			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}

		

	}


	function update_script_form($form_state, $trid, $wiki_url) {
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
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function update_script_form_validate($form, &$form_state){
	
	  	return;
	}
	
	function update_script_form_submit($form, &$form_state){
		
		global $user;

		$query = "UPDATE tutorial_resources SET tutorial_status='pending', tutorial_script='".$form_state['values']['wiki_url']."' where id='".$form_state['values']['trid']."'";

		db_query($query);		

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'script')";


		if(db_query($query)){
			//Mail to contributor
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
			$result = db_query($query);
			$tutorial_detail = db_fetch_object($result);

			$a = new stdClass();
			$a->tc = 'Script';					
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

			drupal_set_message('Tutorial Script Updated succrssfully.');
			$redirect = 'stupload/update_index/?tr='.$form_state['values']['trid'];		

			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while update Script, Please try again.');
		}	

}

?>
