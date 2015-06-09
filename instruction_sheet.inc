<?php

	function instruction_sheets(){
			if(isset($_REQUEST['foss']) && isset($_REQUEST['sheet'])){
				$delete_path = variable_get('file_upload_path', '').'st_videos/'.base64_decode($_REQUEST['foss']).'/'.base64_decode($_REQUEST['foss']).'_'.base64_decode($_REQUEST['sheet']);
				if(file_exists($delete_path)){    
					unlink($delete_path);
        	                }

			}
			$output = '';
			// get all foss
			$query = "select * from foss_categories order by name";
			$result = db_query($query);
			$existing_file = "<div class='existing_file'><h2>Existing instruction sheets</h2><table><tr><th>Foss</th><th>Installation</th><th>Instruction</th><th>Assignment</th></tr>";
			$foss_categories = '';
			$output .= drupal_get_form('instruction_sheet_form', $foss_categories);
			while($row = db_fetch_object($result)){
				$foss_categories[$row->name] .= $row->name;
				// check if file uploaded or not
				$path = variable_get('file_upload_path', '').'st_videos/'.$row->name.'/'.$row->name;
				$link_path = variable_get('tutorial_path', '').'st_videos/'.$row->name.'/'.$row->name;
				//var_dump($path);
				$existing_file .= "<tr><td>".$row->name."</td>";
				
				if(file_exists($path."_Installation_Sheet_English.pdf")){
					$existing_file .= "<td><a target='_blank' href='".$link_path."_Installation_Sheet_English.pdf'>".View."</a> / <a href='/stupload/instruction_sheets?foss=".base64_encode($row->name)."&sheet=".base64_encode('Installation_Sheet_English.pdf')."'>Delete</a></td>";
				}else{
					$existing_file .= "<td>No sheet</td>";
				}
				
				
				if(file_exists($path."_Instruction_Sheet_English.pdf")){
					$existing_file .= "<td><a target='_blank' href='".$link_path."_Instruction_Sheet_English.pdf'>".View."</a> / <a href='/stupload/instruction_sheets?foss=".base64_encode($row->name)."&sheet=".base64_encode('Instruction_Sheet_English.pdf')."'>Delete</a></td>";
				}else{
					$existing_file .= "<td>No sheet</td>";
				}
				if(file_exists($path."_Extra_Assignment_Sheet_English.pdf")){
                                        $existing_file .= "<td><a target='_blank' href='".$link_path."_Extra_Assignment_Sheet_English.pdf'>".View."</a> / <a href='/stupload/instruction_sheets?foss=".base64_encode($row->name)."&sheet=".base64_encode('Extra_Assignment_Sheet_English.pdf')."'>Delete</a></td>";
                                }else{
                                        $existing_file .= "<td>No sheet</td>";
                                }

				$existing_file .= "</tr>";	
			} 
			$output .= $existing_file."</table></div>"; 
			return $output;
	}
	function instruction_sheet_form($form_state) {
		$query = "select * from foss_categories order by name";
		$result = db_query($query);
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

		$form['instruction_sheet'] = array(
			'#type' => 'file',
			'#title' => t('Instruction sheet'),
		);
		$form['installation_sheet'] = array(
			'#type' => 'file',
			'#title' => t('Installation sheet'),
		);
		$form['assignment_sheet'] = array(
                        '#type' => 'file',
                        '#title' => t('Extra Assignment Sheet'),
                );
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function instruction_sheet_form_validate($form, &$form_state){
	
		//var_dump($_FILES['files']);
		// instruction sheet
		/*$file_ext = substr($_FILES['files']['name']['instruction_sheet'], strripos($_FILES['files']['name']['instruction_sheet'], '.'));
		if($_FILES['files']['tmp_name']['instruction_sheet'] == ''){
			//form_set_error('instruction_sheet', t('Please select the instruction file.'));
		}else{
			if($file_ext != '.pdf') {
				form_set_error('instruction_sheet', t('Please select proper file. pdf file only allowed.'));
			}
		}
		
		// installation_sheet
		/*$file_ext = substr($_FILES['files']['name']['installation_sheet'], strripos($_FILES['files']['name']['installation_sheet'], '.'));
		if($_FILES['files']['tmp_name']['installation_sheet'] == ''){
			form_set_error('installation_sheet', t('Please select the installation file.'));
		}else{
			if($file_ext != '.pdf') {
				form_set_error('installation_sheet', t('Please select proper file. zip file only allowed.'));
			}
		}*/
	  	return;
	}
	
	function instruction_sheet_form_submit($form, &$form_state){
		$path = variable_get('file_upload_path', '').'st_videos/'.$form_state['values']['foss_categories'].'/'.$form_state['values']['foss_categories'];
		if($_FILES['files']['size']['instruction_sheet'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['instruction_sheet'], $path."_Instruction_Sheet_English.pdf")){
				drupal_set_message('Instruction sheet uploaded successfully');
			}else{
				drupal_set_message('Instruction sheet upload error, Please try again.');
			}
		}
		
		if($_FILES['files']['size']['installation_sheet'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['installation_sheet'], $path."_Installation_Sheet_English.pdf")){
				drupal_set_message('Installation sheet uploaded successfully');
			}else{
				drupal_set_message('Installation sheet upload error, Please try again.');
			}
		}
		if($_FILES['files']['size']['assignment_sheet'] > 0){
                        if(move_uploaded_file($_FILES['files']['tmp_name']['assignment_sheet'], $path."_Extra_Assignment_Sheet_English.pdf")){
                                drupal_set_message('Extra assignment sheet uploaded successfully');
                        }else{
                                drupal_set_message('Extra assignment sheet upload error, Please try again.');
                        }
                }

	}
?>
