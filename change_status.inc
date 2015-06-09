<?php

	function change_status_form($form_state) {
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
		    '#attributes' => array('class' => 'status_change_lang_dest'),
		    '#options' => '',
		    '#required' => TRUE,
		    '#validated' => TRUE,
		);
		$form['source']['component'] = array(
			'#type' => 'select',
			'#multiple' => true,
			'#title' => t('Component'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('outline' =>'Outline', 'script' => 'Script', 'slide' => 'Slide', 'video' => 'Video', 'code' => 'Codefile', 'assignment' => 'Assignment'),
			'#attributes' => array('class' => 'language_component'),
		);
		$form['source']['change_type'] = array(
			'#type' => 'select',
			'#title' => t('What is to be changed'),
			'#required' => TRUE,
			'#options' => array(0 => 'Tutorial Status' , 1 => 'User ID'),
			//'#attributes' => array('class' => 'upeng_video_status'),
		);
		$form['source']['status'] = array(
			'#type' => 'select',
			'#title' => t('Component'),
			//'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('5' =>'Need Improvement'),
		);
		$form['source']['user'] = array(
			'#type' => 'select',
			'#title' => t('Component'),
			//'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('user' =>'User'),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
	   );
	
	    return $form;
	}
	
	
	function change_status_form_validate($form, &$form_state){
	    if (empty($form_state['values']['component'])){
			form_set_error('component', t('Please select FOSS'));
		}
		if ($form_state['values']['stutorial_name'] == ''){
			form_set_error('stutorial_name', t('Please select tutorial name'));
		}
		if ($form_state['values']['language'] == ''){
			form_set_error('language', t('Please select Language'));
		}
	  	return;
	}
	
	function change_status_form_submit($form, &$form_state){
	    $tcc = '';
	    $tr = '';
	    
	    foreach($form_state['values']['component'] as $c){
	        if(in_array($c, array('slide', 'code', 'assignment'))){
	            if($tcc)
	                $tcc .= ', '; 
	            $tcc .= 'tutorial_'.$c. '_status = '.$form_state['values']['status'];
	        }
	        if(in_array($c, array('outline', 'script', 'video'))){
	            if($tr)
	                $tr .= ', '; 
	            $tr .= 'tutorial_'.$c.'_status = ' .$form_state['values']['status'];
	        }
	    }

	    if($tcc)
            $tcc_query = "update tutorial_common_contents set ".$tcc.' where tutorial_detail_id='.$form_state['values']['stutorial_name'];
            
        if($tr)
            $tr_query = "update tutorial_resources set tutorial_status='pending', ".$tr." where tutorial_detail_id=".$form_state['values']['stutorial_name']." and language = '".$form_state['values']['language']."'";
        else
            $tr_query = "update tutorial_resources set tutorial_status='pending' where tutorial_detail_id=".$form_state['values']['stutorial_name']." and language = '".$form_state['values']['language']."'";
            
        var_dump($tcc_query);
        var_dump($tr_query);
        if($tcc_query){
           if(db_query($tcc_query))
            	drupal_set_message('Status updated');
            else
                drupal_set_message('tutorial commom content update fail !!');
        }
        if($tr_query){
            if(db_query($tr_query))
                drupal_set_message('status updated');
            else
                drupal_set_message('tutorial tutorial resouces fail !!');
		}
	}
?>
