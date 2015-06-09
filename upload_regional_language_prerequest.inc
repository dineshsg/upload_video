<?php

	function upload_regional_language_prerequest_form($form_state) {

		$flag = 1;
		$pos = strpos($_REQUEST['q'], 'update_regional_language_tutorial');
		if ($pos !== false) {
			$flag = 2;
		}

		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		// $upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$tutorial_name = array('' => 'Tutorial Name');

		// select FOSS Category
		$query = "select name from foss_categories order by name";
		$result = db_query($query);
		$foss_category = array('' => 'Select FOSS category');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}
		
		// select Languages
		$query = "select name from tutorial_languages order by name ASC";
		$result = db_query($query);
		$languages = array('' => 'Select language');
		while($row = db_fetch_object($result)){
			if($row->name != 'English'){
				$languages[$row->name] =  $row->name;
			}
		}

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
		
		$form['flag'] = array(
			'#type' => 'hidden',
			'#value' => 0,
			'#attributes' => array('class' => 'upload_flag'),
		);
		$form['redir_flag'] = array(
			'#type' => 'hidden',
			'#value' => $flag
		);
		$form['qval'] = array(
			'#type' => 'hidden',
			'#value' => $_REQUEST['q']
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'uolang_foss_category_name'),
			'#prefix' => '<div class="uolang-category-level-name">',
		);
		
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial level'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array(''=> 'Select Tutorial level'),
			'#attributes' => array('class' => 'uolang_tutorial_level uolang_tutorial_level'),
			'#prefix' => '<div class="uolang-level-name"><div class="uolang-level">',
			'#suffix' => '</div>', 
		);
		
		$form['tutorial_lang'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Languages'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $languages,
			'#attributes' => array('class' => 'uolang_tutorial_lang'),
			'#prefix' => '<div class="uolang-lang">',
			'#suffix' => '</div>', 
		);
		
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('' => 'Tutorial Name'),
			'#attributes' => array('class' => 'uolang_tutorial_name'),
			'#prefix' => '<div class="uolang-name">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_regional_language_prerequest_form_validate($form, &$form_state){

		$flag_val = 0;

		if($form_state['values']['foss_category'] == ''){
			form_set_error('tutorial_name', t('Please select FOSS Category.'));
			$flag_val = 1;
		}
		if($form_state['values']['tutorial_level'] == ''){
			form_set_error('tutorial_level', t('Please select Tutorial Level.'));
			$flag_val = 1;
		}
		if($form_state['values']['tutorial_lang'] == ''){
			form_set_error('tutorial_lang', t('Please select Language.'));
			$flag_val = 1;
		}
		if($form_state['values']['tutorial_name'] == ''){
			form_set_error('tutorial_name', t('Please select Tutorial Name.'));
			$flag_val = 1;
		}
		if($flag_val == 1){
			drupal_goto($form_state['values']['qval'], $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
		return;
	}
	
	function upload_regional_language_prerequest_form_submit($form, &$form_state){
		global $user;
		$query = "select td.id as tdid, tcc.id as tccid from tutorial_details td, tutorial_common_contents tcc where foss_category='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."' and tcc.tutorial_detail_id=td.id";

		$result = db_query($query);
		$row = db_fetch_object($result);

		$tutorial_detail_id = $row->tdid;

		$language = $form_state['values']['tutorial_lang'];
		$common_id = $row->tccid;
		
		$query = "select id from tutorial_resources where tutorial_detail_id=".$tutorial_detail_id." and language='".$language."'";
		$result = db_query($query);
		$row = db_fetch_object($result);
		if($row){
			if($_POST['redir_flag'] == 1){
				drupal_goto('stupload/upload_regional_language_tutorial_index', array('tr' => $row->id), $fragment = NULL, $http_response_code = 302);
			} else{
				drupal_goto('stupload/update_regional_language_tutorial_index', array('tr' => $row->id), $fragment = NULL, $http_response_code = 302);
			}
		} else{
			$query = "INSERT INTO `tutorial_resources` (`id`, `tutorial_detail_id`,`uid`, `tutorial_content_id`, `language`, `tutorial_outline`, `tutorial_script`, `tutorial_script_timed`, `tutorial_video`) VALUES (NULL, '".$tutorial_detail_id."', '".$user->uid."', '".$common_id."', '".$language."', 'pending', 'pending', 'pending', 'pending')";

			if(db_query($query)){
				// resources ID
				drupal_set_message('Tutorial details added successfully');
				$query = "select id from tutorial_resources where tutorial_detail_id='".$tutorial_detail_id."' and language='".$language."'";
				$result = db_query($query);
				$row = db_fetch_object($result);
				if($_POST['redir_flag'] == 1){
					drupal_goto('stupload/upload_regional_language_tutorial_index', array('tr' => $row->id), $fragment = NULL, $http_response_code = 302);
				} else{
					drupal_goto('stupload/update_regional_language_tutorial_index', array('tr' => $row->id), $fragment = NULL, $http_response_code = 302);
				}
			}else {
				drupal_set_message('Error while Tutorial, Please try again.');
			}
		}
	}
?>
