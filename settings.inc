<?php
	function stupload_settings_form($form_state){
		$form['wiki_path'] = array(
			'#type' => 'textfield',
			'#title' => t('Wiki path'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('wiki_path', ''),
		);
		$form['tutorial_path'] = array(
			'#type' => 'textfield',
			'#title' => t('Tutorial path'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('tutorial_path', ''),
		);
		$form['file_upload_path'] = array(
			'#type' => 'textfield',
			'#title' => t('File upload path'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('file_upload_path', ''),
		);
		$form['slide'] = array(
			'#type' => 'textfield',
			'#title' => t('Allowed slide file extensions'),
			'#description' => t('A comma separated list WITHOUT SPACE of slide file extensions that are permitted to be uploaded on the server'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('slide_extensions', ''),
		);
		$form['codefile'] = array(
			'#type' => 'textfield',
			'#title' => t('Allowed code file extensions'),
			'#description' => t('A comma separated list WITHOUT SPACE of code file extensions that are permitted to be uploaded on the server'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('codefile_extensions', ''),
		);
		$form['video'] = array(
			'#type' => 'textfield',
			'#title' => t('Allowed video file extensions'),
			'#description' => t('A comma separated list WITHOUT SPACE of video file extensions that are permitted to be uploaded on the server'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('video_extensions', ''),
		);
		$form['assignment'] = array(
			'#type' => 'textfield',
			'#title' => t('Allowed assignment file extensions'),
			'#description' => t('A comma separated list WITHOUT SPACE of assignment file extensions that are permitted to be uploaded on the server'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('assignment_extensions', ''),
		);
		$form['admin_review_user'] = array(
			'#type' => 'textfield',
			'#title' => t('Admin review user\'s role'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('admin_review_user', ''),
		);
		$form['domain_user'] = array(
			'#type' => 'textfield',
			'#title' => t('Domain review user\'s role'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('domain_user', ''),
		);
		$form['quality_checker'] = array(
			'#type' => 'textfield',
			'#title' => t('Quality review user\'s role'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('quality_checker', ''),
		);
		$form['contributor'] = array(
			'#type' => 'textfield',
			'#title' => t('Contributor\'s role'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('contributor', ''),
		);
		$form['resume_extensions'] = array(
			'#type' => 'textfield',
			'#title' => t('Allowed video file extensions'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('resume_extensions', ''),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	
	function stupload_settings_form_submit($form, &$form_state){
		variable_set('wiki_path', $form_state['values']['wiki_path']);
		variable_set('tutorial_path', $form_state['values']['tutorial_path']);
		variable_set('file_upload_path', $form_state['values']['file_upload_path']);
		variable_set('slide_extensions', $form_state['values']['slide']);
		variable_set('codefile_extensions', $form_state['values']['codefile']);
		variable_set('video_extensions', $form_state['values']['video']);
		variable_set('assignment_extensions', $form_state['values']['assignment']);
		variable_set('admin_review_user', $form_state['values']['admin_review_user']);
		variable_set('domain_user', $form_state['values']['domain_user']);
		variable_set('quality_checker', $form_state['values']['quality_checker']);
		variable_set('contributor', $form_state['values']['contributor']);
		variable_set('resume_extensions', $form_state['values']['resume_extensions']);
		drupal_set_message(t('Settings updated'), 'status');
	}
