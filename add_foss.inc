<?php

	function add_foss_form($form_state) {
    
		$query = "select * from foss_categories order by name";
		$result = db_query($query);
		$foss_categories = array('' => '-- Select --', 'addnew' => '-- Add New FOSS --');
		$tutorial_order_no = array('' => 'Order');
		while($row = db_fetch_object($result)){
			$foss_categories[$row->name] .= $row->name;
		}

		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['foss_categories'] = array(
			'#type' => 'select',
			'#title' => t('Available FOSS'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_add'),
			'#options' => $foss_categories
		);

		$form['name'] = array(
			'#type' => 'textfield',
			'#title' => t('FOSS Category (use a-z, A-Z, - Only)'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_name'),
			'#prefix' => '<div class="aable-foss-name" style="display: none">',
			'#suffix' => '</div>',
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function add_foss_form_validate($form, &$form_state){
	
		$query = "select name from foss_categories where name='".$form_state['values']['name']."'";
		$result = db_query($query);
		$row = db_fetch_object($result);
		if (!preg_match('/^[a-zA-Z-]+$/', $form_state['values']['name'])){
			form_set_error('name', t('Invalid Name, Please use a-z, A-Z, - Only'));
			drupal_goto($path = 'stupload/addfoss', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else{
			if($row){
				form_set_error('name', t('Name already exists.'));
				drupal_goto($path = 'stupload/addfoss', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
		}
		return;
	}
	
	function add_foss_form_submit($form, &$form_state){
		$query = "INSERT INTO `foss_categories` (`id`, `name`) VALUES (NULL, '".$form_state['values']['name']."')";
		if(db_query($query)){
			drupal_set_message('New FOSS Category Added Successfully');
		}else {
			drupal_set_message('Error while Adding FOSS Category, Please try again.');
		}
		drupal_goto($path = 'stupload/add_foss', $query = NULL, $fragment = NULL, $http_response_code = 302);

	}
?>
