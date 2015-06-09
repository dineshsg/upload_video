<?php
	function update_studyplan() {
	
		if($_POST){
			$output = "<h2>Update Study Plan for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('update_studyplan_form', $_POST['tdid'], $_POST['trid']);
			return $output;
		}else{
			$output = "Somthing went wrong. Please try again";
			return $output;
		}


	}

	function update_studyplan_form($form_state, $tdid, $trid) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$studyplan = array('' => 'Select');
		$i = 1;
		while($i < 100){
			$studyplan[$i] = $i;
			$i++;
		}
		
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
		
		$form['tdid'] = array(
			'#type' => 'hidden',
			'#value' => $tdid
		);
		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid
		);
		$form['studyplan'] = array(
			'#type' => 'select',
			'#title' => t('Study Plan'),
			'#required' => TRUE,
			'#options' => $studyplan,
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
		
	return $form;
	
	}
	
	
	function update_studyplan_form_validate($form, &$form_state){
		
		return;
	}
	
	function update_studyplan_form_submit($form, &$form_state){
		
		$query = "UPDATE tutorial_details SET order_code='".$form_state['values']['studyplan']."' where id='".$form_state['values']['tdid']."'";

		if(db_query($query)){
			drupal_set_message('Tutorial Stydy Plan Updated succrssfully.');
			$redirect = 'stupload/update_index/?tr='.$form_state['values']['trid'];		

			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Update studyplan, Please try again.');
		}	

}


?>