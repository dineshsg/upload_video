<?php
        function duration_videos()
        {
	    if(!isset($_REQUEST['foss']))
	    {
			$query="select td.foss_category,td.tutorial_level,td.tutorial_name,tr.tutorial_video,tr.language from tutorial_resources as tr,tutorial_details as td where td.id=tr.tutorial_detail_id AND tr.language='English' AND tr.tutorial_status='accepted' ORDER BY td.foss_category";
	    }
	    else
	    {
            $query="select td.tutorial_level,td.foss_category,td.tutorial_name,tr.tutorial_video,tr.language from tutorial_resources as tr,tutorial_details as td where td.id=tr.tutorial_detail_id AND td.foss_category = '".$_REQUEST['foss']."' AND tr.language='English' AND tr.tutorial_status='accepted'";
            }
		//$query="select td.tutorial_name,tr.tutorial_video,tr.language from tutorial_resources as tr,tutorial_details as td where td.id=tr.tutorial_detail_id";
			$headers = array(t('FOSS'),('Level'),t('Tutorial Name'),t('Tutorial Duration'));
			$result = db_query($query);
			$rows = array();
			$path = "/Sites/spoken_tutorial_org/sites/default/files/";
			while ($item = db_fetch_object($result))
			{
				$movie = new ffmpeg_movie($path.$item->tutorial_video);
				$time = gmdate("H:i:s", $movie->getDuration());
				$rows[] = array(
					$item->foss_category,
					$item->tutorial_level,
					$item->tutorial_name,
					$time,
				);
			}
		    $output = drupal_get_form('duration_videos_form');
		    $output .= theme('table', $headers, $rows);
		    return $output;
        }
		function duration_videos_form($form_state)
		{
		    //var_dump("DInesh");
			$query = "select DISTINCT td.foss_category from tutorial_details as td,tutorial_resources as tr where td.id=tr.tutorial_detail_id AND tr.language='English' ORDER BY td.foss_category";
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
		function duration_videos_form_submit($form, &$form_state)
		{
			$redirect = 'stupload/duration';
			$query = array('foss' => $form_state['values']['foss']);
			drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
		}

?>
