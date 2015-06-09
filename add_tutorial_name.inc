<?php

	function add_tutorial_name_form($form_state) {
		// get all foss categories
		$query = "select * from foss_categories order by name";
		$result = db_query($query);
		$foss_categories = array('' => 'Select FOSS');
		$tutorial_order_no = array('' => 'Order');
		while($row = db_fetch_object($result)){
			$foss_categories[$row->name] = $row->name;
		}
		$i = 1;
		while($i < 100){
			$tutorial_order_no[$i] = $i;
			$i++;
		}
		
		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addtutorial-name');

		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_categories,
			'#attributes' => array('class' => 'uptn_foss_category_name')
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Level'),
			'#required' => TRUE,
			'#options' => array('' => 'Level', 'D0' => 'D0', 'S1' => 'S1', 'C2' => 'C2', 'C3' => 'C3', 'C4' => 'C4', 'C5' => 'C5'),
			'#attributes' => array('class' => 'uptn_tutorial_level')
		);

		$form['available_name'] = array(
			'#type' => 'select',
			'#validated' => TRUE,
			'#title' => t('Available names'),
			'#options' => array('' => 'Select'),
			'#attributes' => array('class' => 'uptn_tutorial_avnames'),
			'#prefix' => '<div class="aable-tutorial-name">',
			'#suffix' => '</div>', 
		);

		$form['tutorial_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Tutorial Name'),
			'#size' => 30,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#attributes' => array('class' => 'uptn_tutorial_name'),
			'#prefix' => '<div class="add-new-tutorial-name">',
			'#suffix' => '</div>', 

		);
		$form['tutorial_order_no'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Order Number'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_order_no,
			'#attributes' => array('class' => 'uptn_tutorial_order_no'),
			'#prefix' => '<div class="poll-form">',
			'#suffix' => '</div>', 
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function add_tutorial_name_form_validate($form, &$form_state){
		if (!preg_match('/[A-Za-z0-9]+/', $form_state['values']['tutorial_name'])){
		//if (!preg_match('/^[0-9a-zA-Z-]+$/', $form_state['values']['tutorial_name'])){
			form_set_error('tutorial_nam', t('Invalid Name, Please use a-z, A-Z, -, 0-9 Only'));
			drupal_goto($path = 'stupload/add_tutorial', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else{
			$query = "select tutorial_name from tutorial_details where foss_category='".$form_state['values']['foss_category']."' and tutorial_level='".$form_state['values']['tutorial_level']."' and tutorial_name='".$form_state['values']['tutorial_name']."'";
			//echo $query;
			//die;
			$result = db_query($query);
			$row = db_fetch_object($result);
			if($row){
				form_set_error('tutorial_name', t('Name already exists.'));
				drupal_goto($path = 'stupload/add_tutorial', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
		}
		
	  	return;
	}
	
	function add_tutorial_name_form_submit($form, &$form_state){
		
		// create Directory
		if (isset($form_state['values']['foss_category']) && isset($form_state['values']['tutorial_level']) && isset($form_state['values']['tutorial_name'])){
			$name = $form_state['values']['foss_category'].'/'.$form_state['values']['tutorial_level'].'/'.$form_state['values']['tutorial_name'].'/'.Resource;
		    if(mkdir (variable_get('file_upload_path', '').'st_videos/'.$name, 0777,true)){
		    	drupal_set_message('Directories created Successfully');
		    	$query = "INSERT INTO `tutorial_details` (`id`, `foss_category`, `tutorial_name`, `tutorial_level`, `order_code`) VALUES (NULL, '".$form_state['values']['foss_category']."', '".$form_state['values']['tutorial_name']."', '".$form_state['values']['tutorial_level']."', '".$form_state['values']['tutorial_order_no']."')";
				if(db_query($query)){
					drupal_set_message('New Tutorial Name Added Successfully');
					//insert record to tutorial resource and tutorial common content table
					$query = "select id as tdid from tutorial_details where foss_category='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' order by tutorial_name";
					$result = db_query($query);
					$tdid = db_fetch_object($result);
					
					// insert in tutorial_commom_content table
					$query = "insert into tutorial_common_contents (tutorial_detail_id, tutorial_slide, tutorial_code, tutorial_assignment) values(".$tdid->tdid.", 'pending', 'pending', 'pending')";
					db_query($query); 
					
					// get commom content id
					$query ="select id as tccid from tutorial_common_contents where tutorial_detail_id='".$tdid->tdid."'";
					$result = db_query($query);
					$tccid = db_fetch_object($result);
					
					// insert into tutorial resource
					
					$query = "insert into tutorial_resources (tutorial_detail_id, language, tutorial_content_id, tutorial_outline, tutorial_script, tutorial_script_timed, tutorial_video) values (".$tdid->tdid.", 'English', ".$tccid->tccid.", 'pending', 'pending', 'pending', 'pending')";
					db_query($query);					
					
					drupal_goto($path = 'stupload/add_tutorial', $query = NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					drupal_set_message('Error while Adding Tutorial Name, Please try again.');
				}
		    }
		}

}


?>
