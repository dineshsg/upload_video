<?php
	
	// Public review form
	function public_review_form($form_state, $tdid, $trid, $video_time){
		if(isset($tdid) && $tdid > 0 && isset($trid) && $trid > 0 && $video_time != '') {
			$path = drupal_get_path('module', 'stupload');
			drupal_add_js($path . '/problem_report.js', 'module');
			
			$time = pr_set_video_time($video_time);
			$form['#attributes'] = array('name' => 'problem_report','enctype' => "multipart/form-data", 'class' => 'problem_report', 'onSubmit' => 'return validate_report();');
		
			$form['tdid'] = array(
				'#type' => 'hidden',
				'#value' => $tdid
			);
			$form['trid'] = array(
				'#type' => 'hidden',
				'#value' => $trid
			);
			$form['resource'] = array(
				'#type' => 'select',
				'#title' => t('Resource'),
				'#options' => array('video' => 'Video', 'script' => 'Script', 'outline' => 'Outline'),
				'#attributes' => array('class' => 'rptutorial-resource'),
				'#prefix' => '<div class="presource-div">',
				'#suffix' => '</div><table class="pritems"><tr><td>Criteria</td><td><span class="left-title">Every Where</span> <span class="right-title">Video Time</span> <span class="min">Min</span> <span class="sec">Sec</span></td></tr>'
			);
			$video_missing_type = array(1 => 'A/V sync mismatch', 2 => 'Poor audio quality', 3 => 'Poor video quality', 4 => 'Difficult words', 5 => 'Improper translation', 6 => 'Abusive / offensive language', 7 => 'Pace of narration is extremely fast', 8 => 'Pace of narration is extremely slow');
			for($i=1; $i< 9; $i++){
				for($j=1; $j<6; $j++){
					if($j==1){
						$form['item'.$i.$j] = array(
							'#type' => 'item',
							'#value' => t($video_missing_type[$i]),
							'#attributes' => array('class' => 'form-checkbox'),
							'#prefix' => '<tr class="items '.$i.$j.'"><td class="item">',
							'#suffix' => "</td>",
						);
					}else{
						$form['item'.$i.$j] = array(
							'#type' => 'item',
							'#value' => t($video_missing_type[$i]),
							'#attributes' => array('class' => 'form-checkbox'),
							'#prefix' => '<tr class="items '.$i.$j.'" style="display:none;"><td class="item">',
							'#suffix' => "</td>",
						);
					}
					$form['item'.$i.$j.'_location'] = array(
					    '#type' => 'radios',
					    '#options' => array(0 => '', 1 => ''),
					    '#attributes' => array('class' => 'item'.$i.$j.'_location'),
					    '#prefix' => '<td class="item1_location prmitem">',
				    );
					$form['item'.$i.$j.'prvideo_min'] = array(
						'#type' => 'select',
						'#options' => $time['min'],
						'#attributes' => array('class' => 'prvideo_min'.$i.$j),
						'#prefix' => "<div class='date-and-time prvideo-time'>",
					);
					$form['item'.$i.$j.'prvideo_sec'] = array(
						'#type' => 'select',
						'#options' => $time['sec'],
						'#attributes' => array('class' => 'prvideo_sec'.$i.$j),
						'#suffix' => "</div></td>"
					);
					$form['item'.$i.$j.'reset'] = array(
						'#type' => 'item',
						'#value' => t('Clear'),
						'#prefix' => "<td><div class = 'reset_item ".$i.$j."'>",
						'#suffix' => "</div></td></tr>",
					);
					/*$form['reset'] = array(
						'#value' => '<input class="form-button" type="reset" value=" Reset " />',
						'#weight' => 1001,
					);*/
				}
				$form['item'.$i.'addmore'] = array(
					'#type' => 'item',
					'#value' => t('add more'),
					'#prefix' => "<tr class='items ".$i."'><td colspan='3' style='padding: 0px;'><div style='text-align:right' class='addmore ".$i."'>",
					'#suffix' => "</div></td></tr>",
				);
				$form['item'.$i.'visible'] = array(
					'#type' => 'hidden',
					'#value' => '1',
					'#attributes' => array('class' => 'item_visible'.$i),
				);
				/*$form['item'.$i.'addmore'] = array(
					'#type' => 'item',
					'#value' => t('add more'),
					'#prefix' => "<tr class='items'><td></td><td></td><td>",
					'#suffix' => "</td></tr>",
				);*/
			    
			}
			$form['missing_comment'] = array(
				'#type' => 'textarea',
				'#title' => t('Enter your comment here'),
				'#prefix' => '</table><div class="rpmissing_comment-div">',
				'#suffix' => '</div>',
			);
			
			$form['item'.$i.'_result'] = array(
		        '#type' => 'radios',
		        '#options' => array(0 => 'Video is perfect or has minor problems. Accept the video.', 1 => 'There are substantial mistakes. Improve the video but let this video be displayed.', 2 => 'There are serious issues, correct the video only then accept it.'),
		        '#attributes' => array('class' => 'item'.$i.'_location prfinal-result'),
		        '#prefix' => '<td class="item1_location prmitem">',
	        );
			$form['save'] = array(
				'#type' => 'submit',
				'#value' => 'Submit Review'
			);
	
			return $form;
		}else {
			drupal_goto($_SERVER['HTTP_REFERER']);
		}
	}

	function public_review_form_submit($form, &$form_state){
		global $user;
		echo "<pre>";
		
		//var_dump($form_state['values']);
		/*foreach($form_state['values'] as $key => $value){
			if(!$value){
				unset($form_state['values'][$key]);
			}
		}*/
		
		//$_SESSION['public_reviewed_value'] = $form_state['values'];
		
		//print_r($_SESSION['public_reviewed_value']);
		//print_r($form_state['values']);
		//var_dump($form_state['values']);
		/*
		//var_dump($form_state['values']['item1']);
		//var_dump($form_state['values']['item1_location']);
		//var_dump($form_state['values']);
		if($form_state['values']['resource'] == 'video'){
            $query_array = array();
		    for($i=1; $i < 6; $i++){
		        if($form_state['values']['item'.$i]){
		            if($form_state['values']['item'.$i.'_location'] && ($form_state['values']['item'.$i.'prvideo_min'] != '00') && ($form_state['values']['item'.$i.'prvideo_sec'] != '00')){
		                $query_array[] = $form_state['values']['item'.$i].', '.$form_state['values']['item'.$i.'_location'].', '.'"00:'.$form_state['values']['item'.$i.'prvideo_min'].':'.$form_state['values']['item'.$i.'prvideo_sec'].'"';
		                //echo "video time selected";
		            }elseif(!$form_state['values']['item'.$i.'_location']){
		                $query_array[] = $form_state['values']['item'.$i].', '.$form_state['values']['item'.$i.'_location'].', '.'"00:00:00"';
		                //echo "elsewhere selected";
		            }else{
		                //echo "time not selected";
		            }
		        }
		    }
		    if(count($query_array) > 0){
		        $query = "insert into tutorial_public_review (uid, trid, date_time, component, comment) values (".$user->uid.", ".$form_state['values']['trid'].", NOW(), '".$form_state['values']['resource']."', '".$form_state['values']['missing_comment']."')";
		        var_dump($query);
		        db_query($query);
		        
		        // get id
		        $query = "select id from tutorial_public_review where uid='".$user->uid."' and trid='".$form_state['values']['trid']."' and component='".$form_state['values']['resource']."' and comment= '".$form_state['values']['missing_comment']."'";
		        $result = db_query($query);
		        if($row = db_fetch_object($result)){
		        //var_dump($query_array);
		            foreach($query_array as $qu){
		                $query = 'insert into tutorial_public_review_video (tutorial_public_review_id, item, everywhere, video_time) values('.$row->id.', '.$qu.')';
		                var_dump($query);
		                db_query($query);
		            }
		            drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
		        }
		    }else{
		        drupal_set_message('Sorry, your comment not valiable');
		    }
			drupal_goto('show_video', array('tr' => $form_state['values']['trid'], 'sval' => '1'));
		}else{
		    $query = "insert into tutorial_public_review (uid, trid, date_time, component, comment) values (".$user->uid.", ".$form_state['values']['trid'].", NOW(), '".$form_state['values']['resource']."', '".$form_state['values']['missing_comment']."')";
		    var_dump($query);
		    db_query($query);
		    drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
			drupal_goto('show_video', array('tr' => $form_state['values']['trid'], 'sval' => '1'));
		}
		*/
		//print_r($_SESSION['public_reviewed_value']);
		
		//die;
		drupal_goto('stupload/pr_get_user', array('form_data' => serialize($form_state['values'])));
		
	}
	
	function pr_set_video_time($video_time){
		$vta = explode(':', $video_time);
		$min = '';
		$sec = '';
		$i=0;
		while($i <= intval($vta[1])){
			if($i < 10){
				$min['0'.$i] = '0'.$i;
			}else{
				$min[$i] = $i;
			}
			$i++;
		}
		$i = 9;
		while($i <= 59){
			if($i < 10){
				$sec['0'.$i] = '00-09';
			}else{
				$sec[$i] = ($i-9).'-'.($i);
			}
			$i = $i + 10;
		}
		$time['min'] = $min;
		$time['sec'] = $sec;

		return $time;
	}
// public review list
function public_review_list(){
	$query = '';
	$sql_count = '';
	$sfoss = isset($_REQUEST['foss']) ? $_REQUEST['foss'] : '';
	$slang = isset($_REQUEST['lang']) ? $_REQUEST['lang'] : '';
	if($sfoss && $slang){
		$query= "select tr.id as trid, td.tutorial_level, td.tutorial_name from tutorial_resources tr, tutorial_details td where tr.tutorial_status='public_review' and td.id=tr.tutorial_detail_id and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
		$sql_count= "select count(*) from tutorial_resources tr, tutorial_details td where tr.tutorial_status='public_review' and td.id=tr.tutorial_detail_id and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
		$result = pager_query($query, 30, 0, $sql_count);
		$row = array();
		$header = array('Level', 'Tutorial Name', 'Action');
		while ($item = db_fetch_object($result)) {
			$link_str = l(t('view'), 'stupload/public_review_view/'. $item->trid , array('attributes' => array('class' => 'view-record', 'title' => 'View')));
			$row[] = array($item -> tutorial_level, $item -> tutorial_name, $link_str);
		
		}

		$output = drupal_get_form('public_review_list_form', $sfoss, $slang);		
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		}else{
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	}else{
		$output = drupal_get_form('public_review_list_form', $sfoss, $slang);
	}
	
	return $output;
}	
function public_review_list_form($form_state, $sfoss, $slang){
	global $user;
	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/stupload-view.css');
	
	//fetching list of foss category details
	$query = "select distinct foss_category from tutorial_details ORDER BY foss_category ASC";
	$result = db_query($query);
	$foss_list = array();
	$foss_list[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss_list[$row->foss_category] = $row->foss_category;
	}
	
	$query ="select distinct tl.name from  tutorial_languages tl, tutorial_resources tr where tr.language=tl.name";
	$result = db_query($query);
	$lang[''] = 'Select Language';
	while($row = db_fetch_object($result)){
		$lang[$row->name] = $row->name;
	}
	unset($lang['English']);
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['foss'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $foss_list,
		'#default_value' => $sfoss,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div style="width: 70%; margin: auto;"><table class="foss_search" align="Center" style="border-collapse: inherit; width: 70%;" cellpadding="7"><tr><td>',
		'#suffix' => '</td>'
	);
	
	$form['lang'] = array(
		'#type' => 'select',
		'#options' => $lang,
		'#default_value' => $slang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<td>',
		'#suffix' => '</td>'
	);
	$form['save'] = array(
		'#type' => 'submit',
		'#value' => 'Search',
		'#attributes' => array('class' => 'form-select locate_tutorial'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table></div>'
	);
	
	return $form;
}

function public_review_list_form_validate($form, &$form_state){

	if($form_state['values']['foss'] == '')
		form_set_error('foss', t('Please select FOSS category'));
	if($form_state['values']['lang'] == '')
		form_set_error('lang', t('Please select language'));
}

function public_review_list_form_submit($form, &$form_state){
	$redirect = 'stupload/public_review_list';
	$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang'] );
	drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
}

function public_review_view($id=NULL){
	if($id){
		$query = "select * from tutorial_public_review where trid=".$id;
		$sql_count="select count(*) from tutorial_public_review where trid=".$id;
		$row = array();
		$header =array(
			array('data'=>'Component', 'field'=>'component', 'sort'=>'asc'),
			array('data'=>'Date/time', 'field'=>'date_time'),
			array('data'=>'Comment', 'field'=>'comment'),
		);
		$query .= tablesort_sql($header);
		$result = pager_query($query, 30, 0, $count_query);
		while ($item = db_fetch_object($result)) {
			if($item -> component == 'video'){
				$row[] = array($item -> component, $item -> date_time, "<p>Video time : ".$item->video_time."</p>".$item -> comment);
			}else{
				$row[] = array($item -> component, $item -> date_time, $item -> comment);
			}
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">There is no comment.</div>';
		}else{
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
	}else{
		$output = "<p>Somthing went worng, Please try again</p>";
	}
	return $output;
}

function pr_get_user_form($form_state){
	// add js file 
		//$path = drupal_get_path('module', 'stupload');
		//drupal_add_js($path . '/stupload.js');
		//drupal_add_css($path . '/stupload.css');
		
		//var_dump($_REQUEST);
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['inform'] = array(
			'#type' => 'radios',
			'#title' => t('Do you want us to inform you when we correct the video ? '),
			'#options' => array(1 => 'Yes', 0 => 'No'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#attributes' => array('class' => 'skype_request auto_form'),
			'#prefix' => '<div class="skype-request">',
			'#suffix' => '</div>'
		);
		
		$form['form_data'] = array(
			'#type' => 'hidden',
			'#validated' => TRUE,
			'#value' => $_REQUEST['form_data'],
		);
		
		/*global $user;
		if(!$user->uid){
			$form['Email'] = array(
				'#type' => 'textfield',
				'#title' => t('Enter your email ID '),
				'#prefix' => '<div class="">',
				'#suffix' => '</div>'
			);
		
			$form['login'] = array(
				'#type' => 'item',
				'#prefix' => '<div class=""><p>OR</p><p><a href="'.base_path().'/login/user">If you have account, Please login to post your comments.</a></p>',
				'#suffix' => '</div>'
			);
		}*/
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
}
function pr_get_user_form_validate($form, &$form_state){
	return;
}
function pr_get_user_form_submit($form, &$form_state){
	global $user;
	if($user->uid){
		if($form_state['values']['inform'] == 1){
			store_public_rating($_POST['form_data'], $user->uid, 1);
			drupal_set_message('Thank you for your feedback');
			drupal_goto('<front>', '');
		}else{
			store_public_rating($_POST['form_data'], $user->uid, 0);
			drupal_set_message('Thank you for your feedback');
			drupal_goto('<front>', '');
		}
	}else{
		if($form_state['values']['inform'] == 1){
			//if($form_state['value']['Email']){
			//	store_public_rating($_POST['form_data'], 0, 1, $form_state['value']['Email']);
			//}
			drupal_goto('stupload/pr_get_user_detail', array('form_data' => $_POST['form_data']));
			
		}else{
			store_public_rating($_POST['form_data'], 0, 0);
			drupal_set_message('Thank you for your feedback');
			drupal_goto('<front>', '');
		}
		
		// redirect path ..
	}
}

function pr_get_user_detail_form($form_state){
	global $user;
	$form['Email'] = array(
		'#type' => 'textfield',
		'#title' => t('Enter your email ID '),
		'#prefix' => '<div class="">',
		'#suffix' => '</div>'
	);
	$form['form_data'] = array(
		'#type' => hidden,
		'#value' => $_REQUEST['form_data'],
	);
	$form['login'] = array(
		'#type' => 'item',
		'#prefix' => '<div class=""><p>OR</p><p><a href="'.base_path().'user/login?form_data='.urlencode($_REQUEST['form_data']).'">If you have account, Please login to post your comments.</a></p>',
		'#suffix' => '</div>'
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
   );
	
	return $form;

}
function pr_get_user_detail_form_submit($form, &$form_state){
	store_public_rating($_POST['form_data'], 0, 1, $form_state['values']['Email']);
	drupal_set_message('Thank you for your feedback');
	drupal_goto('<front>', '');
}



function store_public_rating($form_data, $login, $inform, $email = NULL){
	$form_data = unserialize($form_data);
	global $user;

	if($form_data['resource'] == 'video'){
		for($i=1; $i<9; $i++){
			for($j=1; $j<6;$j++){
				if($form_data['item'.$i.$j.'_location'] != ''){
					if($form_data['item'.$i.$j.'_location'] == 1){
						$query_array[] = $i.', '.$form_data['item'.$i.$j.'_location'].', '.'"00:'.$form_data['item'.$i.$j.'prvideo_min'].':'.$form_data['item'.$i.$j.'prvideo_sec'].'"';
					}else{
						$query_array[] = $i.', '.$form_data['item'.$i.$j.'_location'].', '."'00:00:00'";
					}
				}
			}
		}
	}
	if($form_data['resource'] == 'video'){
		if(count($query_array) > 0){
			$query = "insert into tutorial_public_review (uid, email, trid, date_time, component, comment, inform) values (".$login.", '".$email."', ".$form_data['trid'].", NOW(), '".$form_data['resource']."', '".$form_data['missing_comment']."', ".$inform.")";
			db_query($query);
			// get id
			$query = "select id from tutorial_public_review where uid='".$user->uid."' and trid='".$form_data['trid']."' and component='".$form_data['resource']."' and comment= '".$form_data['missing_comment']."'";
			$result = db_query($query);
			if($row = db_fetch_object($result)){
				foreach($query_array as $qu){
				$query = 'insert into tutorial_public_review_video (tutorial_public_review_id, item, everywhere, video_time) values('.$row->id.', '.$qu.')';
					db_query($query);
				}
				drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
			}else{
				echo "not checking";
			}
		}else{
			drupal_set_message('Sorry, your comment not valiable');
		}


	}else{
		$query = "insert into tutorial_public_review (uid, trid, date_time, component, comment) values (".$user->uid.", ".$form_data['trid'].", NOW(), '".$form_data['resource']."', '".$form_data['missing_comment']."')";
		var_dump($query);
		die;
		db_query($query);
		drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
	}
}


/*
if($form_data['resource'] == 'video'){
		if(count($query_array) > 0){
			$uid =0;
			if($user->uid){
				$uid = $user->uid;
			}
			$query = "insert into tutorial_public_review (uid, email, trid, date_time, component, comment) values (".$uid.", '".$_POST['Email']."', ".$form_data['trid'].", NOW(), '".$form_data['resource']."', '".$form_data['missing_comment']."')";
			db_query($query);
			// get id
			$query = "select id from tutorial_public_review where uid='".$user->uid."' and trid='".$form_data['trid']."' and component='".$form_data['resource']."' and comment= '".$form_data['missing_comment']."'";
			$result = db_query($query);
			if($row = db_fetch_object($result)){
			//var_dump($query_array);
				foreach($query_array as $qu){
				$query = 'insert into tutorial_public_review_video (tutorial_public_review_id, item, everywhere, video_time) values('.$row->id.', '.$qu.')';
					db_query($query);
				}
				drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
			}else{
				echo "not checking";
			}
		}else{
			drupal_set_message('Sorry, your comment not valiable');
		}


		//drupal_goto('show_video', array('tr' => $form_data['trid'], 'sval' => '1'));
		}else{
			$query = "insert into tutorial_public_review (uid, trid, date_time, component, comment) values (".$user->uid.", ".$form_data['trid'].", NOW(), '".$form_data['resource']."', '".$form_data['missing_comment']."')";
			var_dump($query);
			db_query($query);
			drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
		//drupal_goto('show_video', array('tr' => $form_state['values']['trid'], 'sval' => '1'));
	} */
