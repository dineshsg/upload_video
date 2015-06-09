<?php

	function update_prerequisit_form($form_state) {
    
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$tutorial_name = array('' => '-- Select --');
		$query = "select distinct name from foss_categories ORDER BY name ASC";
		$result = db_query($query);
		$foss_category = array('' => '-- Select --');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}

		//Foss Category
		$query = "select distinct tutorial_level from tutorial_details";
		$result = db_query($query);
		$level = array('' => '-- Select --');
		while($row = db_fetch_object($result)){
			$level[$row->tutorial_level] =  $row->tutorial_level;
		}



		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english-timed');
		$form['source'] = array(
			'#type' => 'fieldset',
			'#title' => t('Current tutorial'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
		);
		$form['desti'] = array(
			'#type' => 'fieldset',
			'#title' => t('Pre-requisite Tutorial'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
		);
		$form['source']['sfoss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'source_foss_category'),
		);
		$form['source']['stutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'source_lang'),
		);
		
		
		$form['desti']['dfoss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'desti_foss_category'),
		);
		$form['desti']['dtutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'desti_lang'),
		);
		
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
	   );
	
	return $form;
		
	}
	
	
	function update_prerequisit_form_validate($form, &$form_state){
	
	  	return;
	}
	
	function update_prerequisit_form_submit($form, &$form_state){
		$query = "update tutorial_common_contents set tutorial_prerequisit =".$form_state['values']['dtutorial_name'].", tutorial_prerequisit_uid=tutorial_slide_uid, tutorial_prerequisit_status=4 where tutorial_detail_id=".$form_state['values']['stutorial_name'];

		if(db_query($query)){
			drupal_set_message('Prerequisit updated');
		}else{
			drupal_set_message('Prerequisit not updated');
		}
	}

	function update_keywords_form($form_state) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$tutorial_name = array('' => '-- Select --');
		$query = "select distinct name from foss_categories ORDER BY name ASC";
		$result = db_query($query);
		$foss_category = array('' => '-- Select --');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english-timed');
		$form['source'] = array(
			'#type' => 'fieldset',
			'#title' => t('Tutorial Details'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
		);

		$form['source']['sfoss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'source_foss_category'),
		);

		$form['source']['stutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'source_lang'),
		);

		$form['source']['keywords'] = array(
			'#type' => 'textarea',
			'#title' => t('Keywords (User comma to seperate words)'),
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);

		return $form;
	}
	
	
	function update_keywords_form_validate($form, &$form_state){
		return;
	}
	
	function update_keywords_form_submit($form, &$form_state){
		global $user;
		$query = "update tutorial_common_contents set tutorial_keywords='".$form_state['values']['keywords']."', tutorial_keywords_uid=".$user->uid." where tutorial_detail_id=".$form_state['values']['stutorial_name'];
		//var_dump($query); die;

		if(db_query($query)){
			drupal_set_message('Keywords updated');
		}else{
			drupal_set_message('Keywords not updated');
		}
	}
?>
