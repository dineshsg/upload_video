<?php

	function upload_english_timed_prerequest_form($form_state) {
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
	
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'upeng_foss_category_name_etimed'),
			'#prefix' => '<div class="uenglish-category-level-name">',
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Level'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $level,
			'#attributes' => array('class' => 'upeng_tutorial_level_etimed'),
			'#prefix' => '<div class="uenglish-level-name"><div class="uenglish-level">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'upeng_tutorial_name_etimed'),
			'#prefix' => '<div class="uenglish-name">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_timed_prerequest_form_validate($form, &$form_state){
		if($form_state['values']['tutorial_name'] == ''){
			form_set_error('tutorial_name', t('There is no Tutorial for this combination'));
		}
		if($form_state['values']['tutorial_level'] == ''){
			form_set_error('tutorial_level', t('Please select Level'));
		}
		return;
	}
	
	function upload_english_timed_prerequest_form_submit($form, &$form_state){
		global $user;
		$query = "SELECT tr.id AS trid FROM tutorial_details td, tutorial_resources tr WHERE td.foss_category =  '".$form_state['values']['foss_category']."' AND td.tutorial_name =  '".$form_state['values']['tutorial_name']."' AND td.tutorial_level =  '".$form_state['values']['tutorial_level']."' AND tr.tutorial_detail_id = td.id AND tr.language =  'English'";
		$result = db_query($query);
		$row = db_fetch_object($result);
		if($row->trid){
			drupal_set_message('English Timed Script Selected');

			$redirect = 'stupload/etscript/?tr='.$row->trid;
			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error selecting English Timed Script, Please try again.');
		}
		
	}
?>
