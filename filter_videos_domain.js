/* forum */
$( document ).ready(function() {
    /*$('.show-forum-post').click(function(){
        return false;
        $.ajax({
                type : 'POST',
                url : webroot + "existing_forum_post",
                data : {
	                'tdid' : $('.vtdid').val(),
	                'trid': $('.vtrid').val(),
	                'tcvtime' : $('.cvtime').val()
                },
                success : function(data){
	                output = JSON.parse(data);
	                alert(data);
	                $('.fvideo_min').html(min_html_data);
                }
            });
            return false;
    });*/

	webroot = 'http://www.spoken-tutorial.org/';
	loading_image = "<img src='http://www.spoken-tutorial.org/ajax-loader.gif' />";
	
	$('.foss_category').live("change", function(){

		if($('.foss_category').val() != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "filter/domain_lang",
				data : {
					'foss' : $('.foss_category').val() 
				},
				beforeSend: function() {
					field_data = $('.language').html();
					$('.language').html(loading_image);
				},
				success : function(data){
					$('.language').html(field_data);
					output = JSON.parse(data);
					if(output){
						html_data = "";
						count = 0;
						for (var i=0; i < output.length; i++) {
							html_data += "<option value='"+ output[i][0] +"'>" + output[i][0] + "</option>\n";
							count++;
						}
						if(count > 1) {
							html_data = "<option value=''>Select Language</option>\n"+html_data;
						}
						$('.language').html(html_data);
					}else{
						alert('Error fetching languages, please refresh the page and try again');
					}
				}
			});
		}
	});
	/*
	$('.search_lang').live("change", function() {
		if($('.search_lang').val() != '' && $('.search_foss').val() == '') {
			$.ajax({
				type : 'POST',
				url : webroot + "searchvideos/get_foss",
				data : {
					'lang' : $('.search_lang').val() 
				},
				beforeSend: function() {
					field_data = $('.tutorial-search-foss').html();
					$('.tutorial-search-foss').html(loading_image);
				},
				success : function(data){
					$('.tutorial-search-foss').html(field_data);
					output = JSON.parse(data);
					if(output){
						html_data = "";
						count = 0;
						for (var i=0; i < output.length; i++) {
							html_data += "<option value='"+ output[i] +"'>" + output[i] + "</option>";
							count++;
						}
						if(count > 1) {
							html_data = "<option value=''>Select Foss Category</option>\n<option value='all'>All Foss Categories</option>\n"+html_data;
						}else {
							html_data = "<option value=''>Select Foss Category</option>\n"+html_data;
						}
						$('.search_foss').html(html_data);
					}else{
						alert('Error fetching academic code, please refresh the page and try again');
					}
				}
			});
		}
	});
    */
    $(".toggle-content").hide();
	$(".toggle").click(function () {
		$(this).toggleClass("toggled");
		$(".toggle-content").slideToggle("slow");
	});
 }); 

