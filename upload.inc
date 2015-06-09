<?php

	function upload_resume_form($form_state)
	{
		$form['full_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Full Name'),
			'#required' => TRUE,
		);
		$form['email_id'] = array(
			'#type' => 'textfield',
			'#title' => t('Email ID'),
			'#required' => TRUE,
		);
		$form['mobile_no'] = array(
			'#type' => 'textfield',
			'#title' => t('Mobile No'),
			'#required' => TRUE,
		);
		$form['resume'] = array(
			'#type' => 'file',
			'#title' => t('FOSS'),
			'#required' => TRUE,
		);
		return $form;
	}
?>
