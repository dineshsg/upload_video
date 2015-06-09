<?php
	function upload_olang_outline() {
		if(isset($_REQUEST['trid']) && isset($_REQUEST['redirection'])){
			$output = "<h1 class='title'>Upload Outline for ".$_REQUEST['tn']."</h1>";
			$outline = (trim($_REQUEST['outline']) != 'pending')?$_REQUEST['outline']:'';
			$output .= drupal_get_form('upload_olang_outline_form',  $_REQUEST['trid'], $outline, $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Somthing went wrong. Please, try again.</p>";
			return $output;
		}
		
	}
	function upload_olang_outline_form($form_state, $trid, $outline, $redirection) {
		
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
			'#value' => $trid,
			'#attributes' => array('class' => 'uptn_outline_trid')
		);
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		$form['outline_status'] = array(
			'#type' => 'select',
			'#title' => t('Outline Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required', 1 => 'Will upload later', '2' => 'Get from English'),
			'#attributes' => array('class' => 'uptn_outline_status')
		);
		$form['outline'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#attributes' => array('class' => 'uptn_outline', 'style' => 'width: 700px; float: right; margin-top: 20px;'),
			'#default_value' => $outline,
			'#prefix' => '<div class="stupload-outline">',
			'#suffix' => '</div>'
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_olang_outline_form_validate($form, &$form_state){
	
		if ($form_state['values']['outline_status'] == 0){
			if((strlen($form_state['values']['outline']) < 10)){
				form_set_error('outline', t('Please, enter atleast 10 Characters'));
			}
		}
		
	  	return;
	}
	
	function upload_olang_outline_form_submit($form, &$form_state){

		global $user;
		$tuo = $form_state['values']['outline'];
		$tutorial_outline = $form_state['values']['outline'];

		$query = "UPDATE tutorial_resources SET tutorial_outline_status = 1, tutorial_outline_uid = ".$user->uid." ,tutorial_outline='".db_escape_string($tutorial_outline)."' WHERE id='".$form_state['values']['trid']."'";
		
		if($form_state['values']['outline_status'] == 1){
			$query = "UPDATE tutorial_resources SET tutorial_outline_status = 0, tutorial_outline='pending', tutorial_outline_uid = $user->uid WHERE id='".$form_state['values']['trid']."'";
		}
		
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
		    $role = variable_get('contributor', '');
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			//Mail to domain_user and qc
			$role = variable_get('quality_checker', '');
			$quality_mailid = get_language_role_emails($tutorial_detail->language);

			$role = variable_get('domain_user', '');
			$admin_mailid = get_role_emails($role);

			$mailto = $admin_mailid.$quality_mailid;
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			drupal_set_message('Tutorial Outline Updated Successfully');
			if($form_state['values']['redirection'] == 1){
				drupal_goto('stupload/upload_regional_language_tutorial_index', array('tr' => $form_state['values']['trid']), $fragment = NULL, $http_response_code = 302);
			} elseif($form_state['values']['redirection'] == 2){
				drupal_goto('stupload/update_regional_language_tutorial_index', array('tr' => $form_state['values']['trid']), $fragment = NULL, $http_response_code = 302);
			}elseif($form_state['values']['redirection'] == 3){
	            drupal_goto('stupload/domain_review_tutorial_page', array('trid' => $form_state['values']['trid']) , $fragment = NULL, $http_response_code = 302);
            }
		}else {
			drupal_set_message('Error while Adding Tutorial Outline, Please try again.');
		}
	}
?>
