<?php
	function upload_english_tutprerequisit() {
	    //var_dump($_REQUEST);
		if($_REQUEST['trid'] && $_REQUEST['tccid'] && $_REQUEST['redirection']){		
			$output = "<h2>Upload English Pre-requisite for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_tutprerequisit_form', $_REQUEST['trid'], $_REQUEST['tccid'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}
	function upload_english_tutprerequisit_form($form_state, $trid, $tccid, $redirection) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		
		$tutorial_name = array('' => '-- Select tutorial Foss --');
		$query = "select distinct name from foss_categories ORDER BY name ASC";
		$result = db_query($query);
		$foss_category = array('' => '-- Select foss category --');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}
		
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
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		$form['asgmnt_status'] = array(
			'#type' => 'select',
			'#title' => t('Pre-requisite Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Not required'),
			'#attributes' => array('class' => 'upeng_asgmnt_status'),
		);
		$form['foss'] = array(
			'#type' => 'select',
			'#title' => t('Foss Category'),
			'#options' => $foss_category,
			'#default_value' => $foss_default,
			'#attributes' => array('class' => 'forum_foss_category'),
			'#prefix' => '<div class="stupload-form-asgmnt"><div class="forum-category">',
			'#suffix' => '</div>', 
		);
		$form['tname'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial name'),
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#default_value' => $tutorial_default,
			'#attributes' => array('class' => 'forum_tutorial_name'),
			'#prefix' => '<div class="forum-tutorial-vtime"><div class="forum-tutorial">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_tutprerequisit_form_validate($form, &$form_state){
		if ($form_state['values']['asgmnt_status'] == 0){
			if($_FILES['files']['tmp_name']['assignment'] == ''){
				//form_set_error('codefile', t('Please select the codefile file.'));
			}
		}
		
	  	return;
	}
	
	function upload_english_tutprerequisit_form_submit($form, &$form_state){

		global $user;
		//var_dump($form_state['values']);
		
		if($form_state['values']['asgmnt_status']){
		    $query = "UPDATE tutorial_common_contents SET tutorial_prerequisit_uid=".$user->uid.", tutorial_prerequisit_status=6, tutorial_prerequisit=0 WHERE id=".$form_state['values']['tccid'];
		    db_query($query);
		}else{
		    $query = "UPDATE tutorial_common_contents SET tutorial_prerequisit_uid=".$user->uid.", tutorial_prerequisit_status=4, tutorial_prerequisit=".$form_state['values']['tname']." WHERE id=".$form_state['values']['tccid'];
		    db_query($query);
		}
		
		// to track uploader 
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'Pre-request')";
		db_query($query);
		drupal_set_message('Tutorial Pre-request Added Successfully');
		$query= array('trid' => $form_state['values']['trid']);
		if($form_state['values']['redirection'] == 1){
			drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
		} elseif($form_state['values']['redirection'] == 2){
			drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
		}
	}

?>
