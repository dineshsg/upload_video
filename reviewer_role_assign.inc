<?php

	function domain_lang_role() {
	
		$query= "select u.name as uname, tl.name as tname from  tutorial_domain_reviewer_roles tdrr,  tutorial_languages tl, users u where u.uid=tdrr.uid and tl.id=tdrr.language_id";
		$sql_count = "select count(*) from  tutorial_domain_reviewer_roles tdrr,  tutorial_languages tl, users u where u.uid=tdrr.uid and tl.id=tdrr.language_id";
		$result = pager_query($query, 30, 0, $sql_count);
		$row = array();
		$sno = 1;
		$header = array('Sno', 'Name', 'Language');
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->uname, $item->tname);		
		}

		$output = drupal_get_form('domain_lang_role_form');		
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		}else{
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	
		return $output;
	}

	function domain_lang_role_form($form_state) {
		$query = "select distinct name, id from tutorial_languages ORDER BY name ASC";
		$result = db_query($query);
		$lang = array('' => 'Select Language');
		while($row = db_fetch_object($result)){
			$lang[$row->id] = $row->name;
		}
		$username= array('' => "Select Username");
		$query = "select u.name, u.uid as uid from users u, role r, users_roles ur where r.name='domain_user' and ur.rid=r.rid and u.uid=ur.uid";
		$result = db_query($query);
		$username = array('' => 'Select Username');
		while($row = db_fetch_object($result)){
			$username[$row->uid] = $row->name;
		}
		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['username'] = array(
			'#type' => 'select',
			'#title' => t('Username'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_add'),
			'#options' => $username
		);
		$form['lang'] = array(
			'#type' => 'select',
			'#title' => t('Language'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_add'),
			'#options' => $lang
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function domain_lang_role_form_validate($form, &$form_state){

		return;
	}
	
	function domain_lang_role_form_submit($form, &$form_state){

		$query = "insert into tutorial_domain_reviewer_roles (uid, language_id) values(".$form_state['values']['username'].", ".$form_state['values']['lang'].")";		
		if(db_query($query)){
			drupal_set_message('Role assigned successfully');
		}else {
			drupal_set_message('The Role has already assigned');
		}
		drupal_goto($path = 'stupload/domain_lang_role', $query = NULL, $fragment = NULL, $http_response_code = 302);

	}
	
	
	
	function quality_lang_role() {
	
		$query= "select u.name as uname, tl.name as tname from  tutorial_quality_roles tdrr,  tutorial_languages tl, users u where u.uid=tdrr.uid and tl.id=tdrr.language_id";
		$sql_count = "select count(*) from  tutorial_quality_roles tdrr,  tutorial_languages tl, users u where u.uid=tdrr.uid and tl.id=tdrr.language_id";
		$result = pager_query($query, 30, 0, $sql_count);
		$row = array();
		$sno = 1;
		$header = array('Sno', 'Name', 'Language');
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->uname, $item->tname);		
		}

		$output = drupal_get_form('quality_lang_role_form');		
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		}else{
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	
		return $output;
	}

	function quality_lang_role_form($form_state) {
		$query = "select distinct name, id from tutorial_languages ORDER BY name ASC";
		$result = db_query($query);
		$lang = array('' => 'Select Language');
		while($row = db_fetch_object($result)){
			$lang[$row->id] = $row->name;
		}
		$username= array('' => "Select Username");
		$query = "select u.name, u.uid as uid from users u, role r, users_roles ur where r.name='quality_checker' and ur.rid=r.rid and u.uid=ur.uid";
		$result = db_query($query);
		//$username = array('' => 'Select Language');
		while($row = db_fetch_object($result)){
			$username[$row->uid] = $row->name;
		}
		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['username'] = array(
			'#type' => 'select',
			'#title' => t('Username'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_add'),
			'#options' => $username
		);
		$form['lang'] = array(
			'#type' => 'select',
			'#title' => t('Language'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_add'),
			'#options' => $lang
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function quality_lang_role_form_validate($form, &$form_state){

		return;
	}
	
	function quality_lang_role_form_submit($form, &$form_state){

		$query = "insert into tutorial_quality_roles (uid, language_id) values(".$form_state['values']['username'].", ".$form_state['values']['lang'].")";		
		if(db_query($query)){
			drupal_set_message('Role assigned successfully');
		}else {
			drupal_set_message('The Role has already assigned');
		}
		drupal_goto($path = 'stupload/quality_lang_role', $query = NULL, $fragment = NULL, $http_response_code = 302);

	}
?>
