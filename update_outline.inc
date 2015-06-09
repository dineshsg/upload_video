<?php

	function update_outline_form($form_state)
	{
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		
		$tutorial_name = array('' => '-- Select --');
		$query = "select distinct td.foss_category,td.tutorial_name from tutorial_details td,tutorial_resources tr where td.id = tr.tutorial_detail_id AND tr.tutorial_status ='accepted' ORDER BY td.foss_category ASC";
		//var_dump($query);
		//die;
		$result = db_query($query);
		$foss_category = array('' => '-- Select --');
		while($row = db_fetch_object($result)){
			$foss_category[$row->foss_category] =  $row->foss_category;
		}
		
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
			'#title' => t('Change tutorial status'),
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
			'#attributes' => array('class' => 'source_lang  status_change_lang'),
		);
		$form['source']['language'] = array(
		    '#type' => 'select',
		    '#title' => t('Select language'),
		    '#attributes' => array('class' => 'status_change_lang_dest language'),
		    '#options' => '',
		    '#required' => TRUE,
		    '#validated' => TRUE,
		);
		$form['source']['component'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#attributes' => array('class' => 'upload-outline textarea-processed'),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
	   );
	    return $form;
	}
	function update_outline_form_validate($form, &$form_state){
		if((strlen($form_state['values']['component']) > 10)){
			}else{
				form_set_error('outline', t('Please, enter atleast 10 Characters'));
			}
		return;
	}
	function update_outline_form_submit($form, &$form_state)
	{
		global $user;
		$tutorial_outline = ($form_state['values']['component']);
		$query = "UPDATE tutorial_resources SET tutorial_outline_uid = $user->uid ,tutorial_outline='".db_escape_string($tutorial_outline)."' WHERE tutorial_detail_id='".$form_state['values']['stutorial_name']."' AND language = '".$form_state['values']['language']."'";
		db_query($query);
		$query_tut_det = "SELECT id FROM tutorial_resources WHERE tutorial_detail_id='".$form_state['values']['stutorial_name']."' AND language = '".$form_state['values']['language']."'";
		$result = db_query($query_tut_det);
		$tr = db_fetch_object($result);
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$tr->id."', CURRENT_TIMESTAMP, '".$user->name."', 'outline')";
		drupal_set_message("The Outline has been updated!", 'status');
		//var_dump($query);
		//die;
	}

?>
