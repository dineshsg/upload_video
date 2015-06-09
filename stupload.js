$(document).ready(function(){


	webroot = "http://10.118.248.101/spoken_tutorial_org/";
	loading_image = "<img src='http://10.118.248.101/spoken_tutorial_org/ajax-loader.gif' />";
	wiki_url = "http://script.spoken-tutorial.org/index.php/";
	// add new foss, tutorial name
	$('.reviewer-status').attr('checked',false);
	$('.add-new-tutorial-name').css({'display':'none'});
	$('.uptn_tutorial_level').change(function(){
		foss = $('.uptn_foss_category_name').val();
		level = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/get_tutorial_levels",
			data : {
				'foss' : foss, 'level' : level
			},
			beforeSend: function() {
			    field_data = $('.poll-form').html();
			    $('.poll-form').html(loading_image);
			    name_data = $('.aable-tutorial-name').html();
			    $('.aable-tutorial-name').html('');
			},
			success : function(data){
				output = JSON.parse(data);
				var html_data = "<option val=''>-- Select --</option>";
				var html_data2 = "<option val=''>-- Select --</option><option value=2>-- Add New Tutorial --</option>";
				if(output){
					$('.poll-form').html(field_data);
					// add levels
					for (var i=0; i < output['tlevels'].length; i++)
					{
						html_data += "<option value='"+ output['tlevels'][i] +"'>" + output['tlevels'][i] + "</option>";	
					}
					$('.uptn_tutorial_order_no').html(html_data);
					$('.aable-tutorial-name').html(name_data);
					if(output['tnames']){
						for (var i=0; i < output['tnames'].length; i++)
						{
							html_data2 += "<option value='"+ output['tnames'][i] +"'>" + output['tnames'][i] + "</option>";	
						}
					}
					$('.uptn_tutorial_avnames').html(html_data2);
					//show add new tutorial names
					$('.uptn_tutorial_avnames').change(function(){
						if($(this).val() == 2){
							$('.add-new-tutorial-name').css({'display':'block'});
						}else{
							$('.add-new-tutorial-name').css({'display':'none'});
						}
					});

				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	
	// upload english tutorial prerequest
	$('.upeng_foss_category_name').change(function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/get_category_levels",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.uenglish-level-name').html();
			    $('.uenglish-level-name').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				var html_data = "<option value=''>-- Select --</option>";
				if(output){
					$('.uenglish-level-name').html(field_data);
					for (var i=0; i < output.length; i++)
					{
						html_data += "<option value='"+ output[i].tutorial_level +"'>" + output[i].tutorial_level + "</option>";	
					}
					console.log(html_data);
					$('.upeng_tutorial_level').html(html_data);
					// adding the tutorial name under tutorial level
					$('.upeng_tutorial_level').change(function(){
						level = $(this).val();
						foss = $('.upeng_foss_category_name').val();
						flagcls = $('.flag').val();
						$.ajax({
							type : 'POST',
							url : webroot + "stupload/get_category_names",
							data : {
								'level' : level,
								'foss' : foss,
								'flag' : flagcls,
							},
							beforeSend: function() {
							    field_data = $('.uenglish-name').html();
							    $('.uenglish-name').html(loading_image);
							},
							success : function(data){
								output = JSON.parse(data);
								var html_data = "<option value=''>-- Select --</option>";
								if(output){
									$('.uenglish-name').html(field_data);
									for (var i=0; i < output.length; i++)
									{
										html_data += "<option value='"+ output[i].tutorial_name +"'>" + output[i].tutorial_name + "</option>";	
									}
									console.log(html_data);
									$('.upeng_tutorial_name').html(html_data);
								}else{
									alert('Somthing wrong, Please refresh page');
								}
							}
						});
					});
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	
	// upload regional language tutorials
	$('.uolang_foss_category_name').change(function(){

		foss = $(this).val();

		$.ajax({

			type : 'POST',
			url : webroot + "stupload/get_category_levels",
			data : {
				'foss' : foss
			},

			beforeSend: function() {
				$('.uolang_tutorial_name').html("<option value=''>-- Select --</option>");
			    field_data = $('.uolang-level-name').html();
			    $('.uolang-level-name').html(loading_image);
			},

			success : function(data){

				output = JSON.parse(data);
				var html_data = "<option value=''>-- Select --</option>";

				if(output){

					$('.uolang-level-name').html(field_data);

					for (var i=0; i < output.length; i++) {
						html_data += "<option value='"+ output[i].tutorial_level +"'>" + output[i].tutorial_level + "</option>";
					}

					$('.uolang_tutorial_level').html(html_data);

					// get tutorial names under selected foss, level and languages
					$('.uolang_tutorial_level').change(function(){
						$('.uolang_tutorial_name').html("<option value=''>-- Select --</option>");
						$('.uolang_tutorial_lang').attr("value",'');
					});

					$('.uolang_tutorial_lang').change(function(){

						lang = $(this).val();;
						foss = $('.uolang_foss_category_name').val();
						level = $('.uolang_tutorial_level').val();
						flag = $('.upload_flag').val();

						$.ajax({

							type : 'POST',
							url : webroot + "get_olang_tnames",
							data : {
								'level' : level,
								'foss' : foss,
								'lang' : lang,
								'flag' : flag
							},

							beforeSend: function() {
							    field_data = $('.uolang-name').html();
							    $('.uolang-name').html(loading_image);
							},

							success : function(data){

								output = JSON.parse(data);
								console.log(output);

								var html_data = "<option value=''>-- Select --</option>";

								if(output){

									$('.uolang-name').html(field_data);

									for (var i=0; i < output.length; i++)
									{
										html_data += "<option value='"+ output[i].tutorial_name +"'>" + output[i].tutorial_name + "</option>";	
									}
									$('.uolang_tutorial_name').html(html_data);	// get tutorial names under selected foss, level and languages
								}else{
									alert('Somthing wrong, Please refresh page');
								}
							}
						});
					});
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	
	// field hide and show outline box
	if($('.uptn_outline_status').val() == 0){
		$('div.stupload-outline').css({'display' : 'block'});
	}
	$('.uptn_outline_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-outline').css({'display' : 'block'});
		}else if($(this).val() == 2){
			$('div.stupload-outline').css({'display' : 'block'});
			trid = $('.uptn_outline_trid').val();
			$.ajax({
				type : 'POST',
				url : webroot + "stupload/get_english_ouline",
				data : {
					'trid' : trid
				},
				success : function(data){
					output = JSON.parse(data);
					$('.uptn_outline').text(output.tutorial_outline);
				}
			});
		}else{
			$('div.stupload-outline').css({'display' : 'none'});
		}
	});
	
	// for script file
	path = $('.upeng_script_wiki').val();
	wiki_data = "<iframe width='100%' height='600px' src='"+wiki_url+path+"'></iframe>";
	if($('.upeng_script_status').val() == 0){
		$('div.wiki-script-file').html(wiki_data);
	}
	$('.upeng_script_status').change(function(){
		if($(this).val() == 0){
			$('div.wiki-script-file').html(wiki_data);
		}else{
			$('div.wiki-script-file').html('<p></p>');
		}
	});
	
	// for slide file
	if($('.upeng_slide_status').val() == 0){
		$('div.stupload-form-slide').css({'display' : 'block'});
	}
	$('.upeng_slide_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-slide').css({'display' : 'block'});
		}else{
			$('div.stupload-form-slide').css({'display' : 'none'});
		}
	});

	// for video file
	if($('.upeng_video_status').val() == 0){
		$('div.stupload-form-video').css({'display' : 'block'});
	}
	$('.upeng_video_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-video').css({'display' : 'block'});
		}else{
			$('div.stupload-form-video').css({'display' : 'none'});
		}
	});

	// for codefile file
	if($('.upeng_codefile_status').val() == 0){
		$('div.stupload-form-codefile').css({'display' : 'block'});
	}
	$('.upeng_codefile_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-codefile').css({'display' : 'block'});
		}else{
			$('div.stupload-form-codefile').css({'display' : 'none'});
		}
	});
	
	// for assignment file
	if($('.upeng_asgmnt_status').val() == 0){
		$('div.stupload-form-asgmnt').css({'display' : 'block'});
	}
	$('.upeng_asgmnt_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-asgmnt').css({'display' : 'block'});
		}else{
			$('div.stupload-form-asgmnt').css({'display' : 'none'});
		}
	});
	
	// foss category
	$('.foss_category_add').change(function(){
		if($(this).val() == 'addnew'){
			$('.aable-foss-name').css({'display':'block'});
		}else{
			$('.aable-foss-name').css({'display':'none'});
		}
	});
	
	// review need improvement comments
	$('.reviewer-comment').css({'display':'none'});
	$('.reviewer-status').change(function(){
		if($(this).val() == 5){
			$('.reviewer-comment').css({'display':'block'});
		}
		//if($(this).val() == 4){
		if(($(this).val() == 2)||($(this).val() == 4)||($(this).val() == 3)){
			$('.reviewer-comment').css({'display':'none'});
		}
	});
	
	// englished timed
	$('.upeng_tutorial_level_etimed').change(function(){
		level = $(this).val();
		foss = $('.upeng_foss_category_name_etimed').val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/get_etimed_tnames",
			data : {
				'level' : level,
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.uenglish-name').html();
			    $('.uenglish-name').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				var html_data = "<option value=''>-- Select --</option>";
				if(output){
					$('.uenglish-name').html(field_data);
					for (var i=0; i < output.length; i++)
					{
						html_data += "<option value='"+ output[i].tutorial_name +"'>" + output[i].tutorial_name + "</option>";	
					}
					console.log(html_data);
					$('.upeng_tutorial_name_etimed').html(html_data);
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	
// tutorial pre-request
	
	$('.forum_foss_category').live("change", function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/prerequisit_tutorial_name",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.forum-tutname').html();
			    $('.forum-tutname').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				$('.forum-tutname').html(field_data);
				var html_data = '<option value="">-- Select tutorial --</option>';
				for(key in output){
				    html_data += "<option value='"+ key +"'>" + output[key] + "</option>";
				}
				$('.forum_tutorial_name').html(html_data);
			}
		});
	});
	/*  load tutorial name  */
	$('.source_foss_category').live("change", function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/load_tutorial_name",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
                            field_data = $('#edit-stutorial-name-wrapper').html();
                            $('#edit-stutorial-name-wrapper').html(loading_image);
                        },

			success : function(data){
				output = JSON.parse(data);
				var html_data = '<option value="">-- Select tutorial --</option>';
				for(key in output){
				    html_data += "<option value='"+ key +"'>" + output[key] + "</option>";
				}
				$('#edit-stutorial-name-wrapper').html(field_data);
				$('.source_lang').html(html_data);
			}
		});
	});
	/* load tutorial name */
	$('.desti_foss_category').live("change", function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/load_tutorial_name",
			data : {
				'foss' : foss
			},
			 beforeSend: function() {
                            field_data = $('#edit-dtutorial-name-wrapper').html();
                            $('#edit-dtutorial-name-wrapper').html(loading_image);
                        },

			success : function(data){
				output = JSON.parse(data);
				var html_data = '<option value="">-- Select tutorial --</option>';
				for(key in output){
				    html_data += "<option value='"+ key +"'>" + output[key] + "</option>";
				}
				 $('#edit-dtutorial-name-wrapper').html(field_data);
				$('.desti_lang').html(html_data);
			}
		});
	});  
	/* change the component */
	$('.status_change_lang_dest').live("change", function(){
		lang = $(this).val();
		if(lang != 'English'){
			var html_data = '<option value="outline">Outline</option><option value="script">Script</option><option value="video">Video</option>';
		}else{
		    var html_data = '<option value="outline">Outline</option><option value="script">Script</option><option value="slide">Slide</option><option value="video">Video</option><option value="code">Codefile</option><option value="assignment">Assignment</option>';
		}
		$('.language_component').html(html_data);
	});
	
	/*  tutorial change load language */
	$('.status_change_lang').live("change", function(){
		tutname = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/tutchange_get_lang",
			data : {
				'tutname' : tutname
			},
			beforeSend: function() {
                            field_data = $('#edit-language').html();
                            $('#edit-language').html(loading_image);
                        },

			success : function(data){
				output = JSON.parse(data);
				var html_data = '<option value="">-- Select language --</option>';
				for(key in output){
				    html_data += "<option value='"+ output[key] +"'>" + output[key] + "</option>";
				}
				$('#edit-language').html(field_data);
				$('.status_change_lang_dest').html(html_data);
			}
		});
	});
	
	$('.language').live("change", function(){
		tut_val = $('.status_change_lang').val();
		tut_lang = $('.status_change_lang_dest').val();
		$.ajax({
			type : 'POST',
			url : webroot + "stupload/get_english_outline",
		data : {
			'tut_val' : tut_val,
			'tut_lang' : tut_lang
		},
		success : function(data){
			output = JSON.parse(data);
			//console.log(output.tutorial_outline);
			$('.upload-outline').text(output.tutorial_outline);
		}
		});
	});
	$('#edit-change-type').change(function(){
		value = $('#edit-change-type').val();
		//alert(value);
		if(value == '0')
		{
			$('#edit-status-wrapper').css({'display':'none'});
			$('#edit-user-wrapper').css({'display':'block'});
		}
		else
		{
			$('#edit-user-wrapper').css({'display':'none'});
			$('#edit-status-wrapper').css({'display':'block'});
		}
	});
});
