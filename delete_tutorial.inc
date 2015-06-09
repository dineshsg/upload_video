<?php
	function delete_tutorial($params, $tname){
		if(isset($_POST['op']) && $_POST['op']=='Delete'){
				$result = db_query('select language, tutorial_content_id FROM {tutorial_resources} WHERE  id= %d', $_POST['trid']);
				$row = db_fetch_object($result);
				if($row->language == 'English'){
					if(db_query('DELETE FROM {tutorial_common_contents} WHERE  id= %d', $row->tutorial_content_id)){
						db_query('DELETE FROM {tutorial_resources} WHERE  id= %d', $_POST['trid']);
					    drupal_set_message(t('The Tutorial "<b>%name</b>" has been removed.', array('%name' => $_POST['tname'])));
					    cache_clear_all();
					    drupal_goto($path = 'stupload/uploaded_tutorials_list', $query = NULL, $fragment = NULL, $http_response_code = 302);
					}
				}
				else{
					db_query('DELETE FROM {tutorial_resources} WHERE  id= %d', $_POST['trid']);
				    drupal_set_message(t('The Tutorial "<b>%name</b>" has been removed.', array('%name' => $_POST['tname'])));
				    cache_clear_all();
				    drupal_goto($path = 'stupload/uploaded_tutorials_list', $query = NULL, $fragment = NULL, $http_response_code = 302);
				}
		}
		$output = '';
		$output .= drupal_get_form('delete_tutorial_record_form', $params, $tname);
		return $output;

	}

	function delete_tutorial_record_form(&$form, $trid, $tname){
		$form['trid'] = array('#type' => 'hidden', '#value' => $trid);
		$form['tname'] = array('#type' => 'hidden', '#value' => $tname);
		return confirm_form($form, t('Are you sure you want to delete this tutorial <b>"%name"</b>?', array('%name' => $tname)), 'stupload/uploaded_tutorials_list', '', t('Delete'), t('Cancel'));
	}
?>