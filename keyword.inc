<?php
        function list_of_keywords()
        {

            //var_dump($_REQUEST['foss']);
            
            $query="select td.tutorial_name,tc.tutorial_keywords from tutorial_details as td, tutorial_common_contents as tc where td.id=tc.tutorial_detail_id AND td.foss_category = '".$_REQUEST['foss']."'";
			$headers = array(t('Tutorial Name'),t('Tutorial Keyword'));
			$result = db_query($query);
			$rows = array();
			while ($item = db_fetch_object($result))
			{
				$rows[] = array(
					$item->tutorial_name,
					$item->tutorial_keywords,
				);
			    }
		    $output = drupal_get_form('list_of_keywords_form');
		    $output .= theme('table', $headers, $rows);
		    return $output;
        }
		function list_of_keywords_form($form_state)
		{
		    //var_dump("DInesh");
			$query = "select DISTINCT td.foss_category from tutorial_details as td, tutorial_common_contents as tc where td.id=tc.tutorial_detail_id ORDER BY td.foss_category";
	        $result = db_query($query);
        	$foss_list = array();   
        	
        	while ($item = db_fetch_object($result)) {
		        $foss_list[$item -> foss_category] = $item -> foss_category;
	        }
	        //var_dump($foss_list);die;
	        $form['#attributes'] = array('enctype' => "multipart/form-data");
	        $form['foss'] = array(
		        '#type' => 'select',
		        '#title' => t('Foss list'),
		        '#options' => $foss_list,
	        );
	        $form['Submit'] = array(
		'#type' => 'submit',
		'#value' => 'Search',
	);
	        return $form;
		}
		function list_of_keywords_form_submit($form, &$form_state)
		{
			$redirect = 'stupload/keyword';
			$query = array('foss' => $form_state['values']['foss']);
			drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
		}

?>
