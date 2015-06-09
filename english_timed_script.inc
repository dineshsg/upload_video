<?php
	function english_timed_script() {
		if($_REQUEST['trid'] && $_REQUEST['td_path'] && $_REQUEST['flag']){		
			$output = "<h2>Upload English Script for ".$_REQUEST['tn']."</h2>";
			//require_once('path.config.inc');
			$wiki_url = $_REQUEST['td_path'].'English-timed';		
			$output .= "<div class='wiki-script-file'></div>";
			$output .= drupal_get_form('english_timed_script_form', $_REQUEST['trid'], $wiki_url, $_REQUEST['td_path'], $_REQUEST['flag']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}

	function english_timed_script_form($form_state, $trid, $wiki_url, $td_path, $flag) {
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
			'#value' => $td_path
		);

		$form['flag'] = array(
			'#type' => 'hidden',
			'#value' => $flag,
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
	
	
	function english_timed_script_form_validate($form, &$form_state){
	
	  	return;
	}
	
	function english_timed_script_form_submit($form, &$form_state){
		
		global $user;
		$tutorial_script = $form_state['values']['wiki_url'];
		if($form_state['values']['script_status'] == 1){
			$tutorial_script = 'pending';
		}
		$query = "UPDATE tutorial_resources SET tutorial_script_timed='".$tutorial_script."' where id='".$form_state['values']['trid']."'";
		db_query($query);
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'script')";
		if(db_query($query)){
			drupal_set_message('Tutorial script Updated Successfully');
			$query= array('trid' => $form_state['values']['trid']);
			if($form_state['values']['flag'] == 1){
				drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			} elseif($form_state['values']['flag'] == 2){
				drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			}
		}else {
			drupal_set_message('Error while Adding Tutorial Script, Please try again.');
		}
	}
?>
