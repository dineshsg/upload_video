<?php

	function upload_english_prerequest_form($form_state) {
	
		$flag = 1;
		$pos = strpos($_REQUEST['q'], 'update_english_tutorial');
		if ($pos !== false) {
			$flag = 2;
		}
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$tutorial_name = array('' => '-- Select --');
		$query = "select distinct name from foss_categories ORDER BY name ASC";
		$result = db_query($query);
		$foss_category = array('' => '-- Select --');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
		
		$form['flag'] = array(
			'#type' => 'hidden',
			'#value' => $flag,
			'#attributes' => array('class' => 'flag')
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'upeng_foss_category_name'),
			'#prefix' => '<div class="uenglish-category-level-name">',
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Level'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array(''=> '-- Select --'),
			'#attributes' => array('class' => 'upeng_tutorial_level'),
			'#prefix' => '<div class="uenglish-level-name"><div class="uenglish-level">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'upeng_tutorial_name'),
			'#prefix' => '<div class="uenglish-name">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_prerequest_form_validate($form, &$form_state){
		if($form_state['values'][''] == 'foss_category'){
			form_set_error('foss_category', t('Please select foss'));
		}
		if($form_state['values']['tutorial_level'] == ''){
			form_set_error('tutorial_level', t('Please select Level'));
		}
		if($form_state['values']['tutorial_name'] == ''){
			form_set_error('tutorial_name', t('Please select tutorial'));
		}
		return;
	}
	
	function upload_english_prerequest_form_submit($form, &$form_state){
		$query = "select tr.id as trid from tutorial_details td, tutorial_resources tr where td.foss_category='".$_POST['foss_category']."' and td.tutorial_level='".$_POST['tutorial_level']."' and td.tutorial_name='".$_POST['tutorial_name']."' and tr.tutorial_detail_id=td.id";
		$result = db_query($query);
		$trid = db_fetch_object($result);
		if($trid->trid){
			drupal_set_message('New Tutorial Added Successfully');
			$query = array('trid' => $trid->trid);
			if($_POST['flag'] == 1){
				drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			} else{
				drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
			}
		}else {
			drupal_set_message('Error while Adding Tutorial, Please try again.');
		}
		

}


?>
