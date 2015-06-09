<?php
function play_video() {

	require_once('path.config.inc');
	if(isset($_REQUEST['tr'])){
		$query = "select tr.id as trid, tr.language, tr.upload_time, tr.reviewer, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_script_timed, tr.tutorial_video, tc.tutorial_slide, tc.id as tcid, tc.tutorial_code, tc.tutorial_assignment, ";
		$query .= "td.id as tdid, td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code";
		$query .= " from tutorial_resources tr, tutorial_common_contents tc, tutorial_details td where tr.id=".$_REQUEST['tr']." and tc.id=tr.tutorial_content_id and td.id=tr.tutorial_detail_id";
		$result = db_query($query);
		$value = db_fetch_object($result);

		$path = drupal_get_path('module', 'stupload');
		drupal_add_css($path.'/video-js.css');
		drupal_add_js($path.'/tinybox.js');
		drupal_add_js($path.'/video.js');

		$output = '<form>';
		$output .= '<div style="text-align: right"><a href="view_tutorials_list">Back to search</a></div>';
		$output .= '<table border="0" align="left" cellpadding="2" cellspacing="2" width="100%">';
		$output .= '<tr><td><table border="1" cellpadding="10" width="100%">';
		$output .= '<tr><td>FOSS Category</td><td><b>'.$value->foss_category.'</b></td</tr>';
		$output .= '<tr><td>Language</td><td><b>'.$value->language.'</b></td</tr>';
		$output .= '<tr><td>Tutorial Name</td><td><b>'.$value->tutorial_name.'</b></td</tr>';
		$output .= '<tr><td>Upload Time</td><td><b>'.$value->upload_time.'</b></td</tr>';
		$output .= "</table></td></tr>";
		if ($value -> tutorial_outline != 'pending'){
			$output .= '<tr><td><h2>Tutorial Outline:</h2><br><textarea rows=8 cols=75 readonly=true wrap=hard>' . $value -> tutorial_outline . '</textarea></td></tr>';
		}else{
			$output .= '<tr><td><h2>Tutorial Outline:</h2><br><textarea rows=8 cols=75 readonly=true wrap=hard>Tutorial outline currently not available</textarea></td></tr>';
		}
		//$output .= '<tr><td><h2>Pre-Requisites:</h2><br>'.get_pre_reqs($value).'</td></tr>';
		$output .= '<tr><td><br><h3 style="float: left; padding-right: 10px; margin-top: 0px;">Download Links:</h3> ';
		if ($value -> tutorial_assignment != 'pending' && $value -> tutorial_assignment != 'notrequired'){
			$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'" . $tutorial_path.$value -> tutorial_assignment . "'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>';
		}elseif ($value -> tutorial_assignment == 'pending') {
			$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>';
		}else{
			$output .= '<a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_required'" . ", 'Assignment', 'width=600, height=400'); " . 'return false;">Assignment</a>';
		}
		if ($value -> tutorial_code != 'pending' && $value -> tutorial_code != 'notrequired'){
			$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . $tutorial_path.$value -> tutorial_code . "'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>';
		}elseif ($value -> tutorial_code == 'pending') {
			$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>';
		}else{
			$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_required'" . ", 'Codefiles', 'width=600, height=400'); " . 'return false;">Code files</a>';
		}
		$output .= ' | <a href="' . $wiki_path.$value -> tutorial_script . '" target="_blank">Script</a>';
		if($value->language == "English"){
			if ($value -> tutorial_script_timed != 'pending'){
                		$output .= ' | <a href="' . $wiki_path.$value -> tutorial_script_timed . '" target="_blank">Script-Timed</a>';
                	}else{
                		$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Script', 'width=600, height=400'); " . 'return false;">Script-Timed</a>';
                        }
		}

		if ($value -> tutorial_slide != 'pending'){
        	$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'" . $tutorial_path.$value -> tutorial_slide . "'" . ", 'Slides', 'width=600, height=400'); " . 'return false;">Slides</a>';
        }else{
        	$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."resource_not_available'" . ", 'Slides', 'width=600, height=400'); " . 'return false;">Slides</a>';
        }
		$output .= ' | <a href="javascript: void(0)" onclick="window.open(' . "'".$GLOBALS['base_path']."data/download_video.php?trid=" .$value->trid. "', 'Slides', 'width=600, height=400'); " . 'return false;">Video</a>';
		$output .= '</td></tr>';
		if ($value -> tutorial_video != 'pending'){
			$output .= '<tr><td>
						  <div style="margin-top : 10px; margin-bottom : 10px;">
							<video id="example_video_1" class="video-js vjs-default-skin" controls width="600" height="350" data-setup="{}">
					    		<source src="'.$tutorial_path.$value -> tutorial_video.'" type="video/ogg" />
					  		</video>
						  </div>
						</td></tr>';
		}else{
			$output .= '<tr><td style="text-align: center;">Video is currently not available</td></tr>';
		}
		$output .= "</table></form>";
		return $output;
	}else {
		drupal_goto('listvideos');
	}
}

// function get_pre_reqs($value) {
// 	
	// $query = "select td.foss_category, td.tutorial_level, td.tutorial_name, tr.id as tid, tr.language from tutorial_details td, tutorial_resources tr where td.id=tr.tutorial_detail_id and td.foss_category='".$value->foss_category."' and td.tutorial_level='".$value->tutorial_level."' and tr.language='".$value->language."' and td.order_code < ".$value->order_code;
	// // //$query = "SELECT * FROM {stcontentlist} WHERE FOSS_Category = '" . $value -> FOSS_Category . "' AND Language = '" . $value -> Language . "' AND Tutorial_Level='" . $value -> Tutorial_Level . "' AND spkey <" . $value -> spkey . " ORDER BY Tutorial_Level, spkey DESC LIMIT 4";
// 
	// $result = db_query($query);
	// $count = 0;
	// $content = '<table width="100%"><tr><th>FOSS Category</th><th>Level</th><th>Language</th><th colspan=2>Tutorial Name</th></tr>';
	// while ($get_record = db_fetch_object($result)) {
		// ++$count;
		// $content .= '<tr><td>' . $get_record -> foss_category . '</td><td>' . $get_record -> tutorial_level . '</td><td>' . $get_record -> language . '</td><td>' . $get_record -> tutorial_name . '</td><td>';
		// $content .= '<form method="post" action="'.$GLOBALS['base_path'].'play_video" style="margin-bottom: 1px; margin-top: 1px;"><input type="hidden" name="tr" value="'.$get_record->tid.'" /><input type="submit" value="View"></form></td></tr>';
	// }
// 	
	// if ($count == 0)
		// $content = "<table><tr><td>There is no pre-requisites for this video.</td></tr>";
	// $content .= '</table>';
	// return $content;
// }
