<?php
	function upload_summary($params) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		drupal_add_js($path . '/video.js');
		drupal_add_css($path . '/video-js.css');

		if($params){
			$params = parse_url($params);
			$tr_data = explode('=', $params['query']);

			$trid = $tr_data[1];

			$query = "select td.foss_category, td.tutorial_name, td.tutorial_level, td.order_code, tr.language, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_video, tcc.tutorial_slide, tcc.tutorial_code, tcc.tutorial_assignment from tutorial_details td ,tutorial_resources tr, tutorial_common_contents tcc where td.id=tr.tutorial_detail_id and tr.tutorial_content_id=tcc.id and tr.id='".$trid."'";
			$result = db_query($query);
			$tutorial = db_fetch_object($result);

			require_once('path.config.inc');

			//for codefile
			if($tutorial->tutorial_code != 'pending' && $tutorial->tutorial_code != 'notrequired' && file_exists($file_upload_path.$tutorial->tutorial_code)){

				$codefile = read_ziparchive($file_upload_path.$tutorial->tutorial_code);
			}else{
				$codefile = $tutorial->tutorial_code;
			}

			// for assignment

			if($tutorial->tutorial_assignment != 'pending' && $tutorial->tutorial_assignment != 'notrequired' && file_exists($file_upload_path.$tutorial->tutorial_assignment)){
				$file_ext = substr($tutorial->tutorial_assignment, strripos($tutorial->tutorial_assignment, '.'));
				$file_name = str_replace('/', '', substr($tutorial->tutorial_assignment, strripos($tutorial->tutorial_assignment, '/')));
				if($file_ext == '.txt' || $file_ext == '.pdf'){
					$assign = "<a href='".$tutorial_path.$tutorial->tutorial_assignment."' target='_blank'>".$file_name."</a>";
				}
			}else{
				$assign = $tutorial->tutorial_assignment;
			}

			// for slide
			if($tutorial->tutorial_slide != 'pending' && file_exists($file_upload_path.$tutorial->tutorial_slide)){
				$slide = read_ziparchive($file_upload_path.$tutorial->tutorial_slide);
			}else{
				$slide = $tutorial->tutorial_slide;
			}

			// video
			if($tutorial->tutorial_video != 'pending' && file_exists($file_upload_path.$tutorial->tutorial_video)){
				$video = '<video id="example_video_1" class="video-js vjs-default-skin" controls width="400" height="300" data-setup="{}">
				    		<source src="'.$tutorial_path.$tutorial->tutorial_video.'" type="video/mp4" />
				  		</video>';
			}else{
				$video = $tutorial->tutorial_slide;
			}
			// script
			if($tutorial->tutorial_script != 'pending'){
				$script = "<iframe width='100%' height='100%' src='".$wiki_path.$tutorial->tutorial_script."'></iframe>";
			}else{
				$script = $tutorial->tutorial_script;
			}


			$output = "<div class='stupload-form'>";
				$output .= "<div class='summery'>";
					$output .= "<div class='summery-title-content'>";
						$output .="<span class='left'>Foss Category</span><span class='right'>: ".$tutorial->foss_category."</span>";
						$output .="<span class='left'>Tutorial Name</span><span class='right'>: ".$tutorial->tutorial_name."</span>";
						$output .="<span class='left'>Tutorial Level</span><span class='right'>: ".$tutorial->tutorial_level."</span>";
					$output .= "</div>";
					$output .= "<div class='left-content'>";
						$output .= "<div class='summery-outline'>";
							$output .= "<p class='summery-title'>Outline</p>";
							$output .= "<p class='summery-content'><textarea rows='6' cols='30' readonly=readonly>".$tutorial->tutorial_outline."</textarea></p>";
						$output .= "</div>";
						$output .= "<div class='summery-slide'>";
							$output .= "<p class='summery-title'>Slide</p>";
							$output .= "<p class='summery-content'>".$slide."</p>";
						$output .= "</div>";
						$output .= "<div class='summery-codefile'>";
							$output .= "<p class='summery-title'>Codefile</p>";
							$output .= "<p class='summery-content'>".$codefile."</p>";
						$output .= "</div>";
						$output .= "<div class='summery-assignment'>";
							$output .= "<p class='summery-title'>Assignment</p>";
							$output .= "<p class='summery-content'>".$assign."</p>";
						$output .= "</div>";
					$output .= "</div>";
					$output .= "<div class='right-content'>";
						$output .= "<div class='summery-video'>";
							$output .= "<p class='summery-title'>Video</p>";
							$output .= "<p class='summery-content'>".$video."</p>";
						$output .= "</div>";
					$output .= "</div>";
					$output .= "<div class='summery-script'>";
						$output .= "<p class='summery-title'>Script</p>";
						$output .= "<p class='summery-content'>".$script."</p>";
					$output .= "</div>";
				$output .= "</div>";
			$output .= "</div>";
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}


	}


?>