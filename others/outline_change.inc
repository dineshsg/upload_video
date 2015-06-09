<?php 
	function outline_change_form($form_state){
		
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		
		$query = "select distinct foss_category from tutorial_details order by foss_category ASC";
		$result = db_query($query);
		$foss = array();
		$foss[''] = "Select Foss Category";
		while($row = db_fetch_object($result)){
			$foss[$row->foss_category] = human_text($row->foss_category);
		}
		
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['foss'] = array(
			'#type' => 'select',
			'#title' => t('Foss Category'),
			'#options' => $foss,
			'#attributes' => array('class' => 'outline_change_foss'),
		);
		$form['language'] = array(
			'#type' => 'select',
			'#title' => t('Language'),
			'#validated' => TRUE,
			'#options' => array('' => 'Select Language'),
			'#attributes' => array('class' => 'outline_change_language'),
			'#prefix' => '<div class="outline_change_language_div">',
			'#suffix' => '</div>'
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#validated' => TRUE,
			'#options' => array('' => 'Select Tutorial Name'),
			'#attributes' => array('class' => 'outline_change_tutorial_name'),
			'#prefix' => '<div class="outline_change_tutorial_name_div">',
			'#suffix' => '</div>'
		);
		$form['trid'] = array(
			'#type' => 'hidden',
			'#title' => t('Tutorial Resource id'),
			'#attributes' => array('class' => 'outline_change_trid')
		);
		$form['tutorial_outline'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#attributes' => array('class' => 'outline_change_tutorial_outline', 'rows' => '20', 'cols' => '90'),
			'#prefix' => '<div class="outline_change_tutorial_outline_div">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Update')
		);
		
		return $form;
	}

	function outline_change_form_validate($form, &$form_state){
		if($form_state['values']['trid'] == '' || $form_state['values']['tutorial_outline'] == ''){
			drupal_set_error('Please select all dropdowns below.');
		}
	}
	
	function outline_change_form_submit($form, &$form_state){
		$query = "update tutorial_resources set tutorial_outline='".$form_state['values']['tutorial_outline']."' where id=".$form_state['values']['trid'];
		$result = db_query($query);
		if($result){
			drupal_set_message('Outline updated successfully!');
		}else{
			drupal_set_error('Something went wrong!');
		}
	}
