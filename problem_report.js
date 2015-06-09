$(document).ready(function(){
	webroot = "http://"+location.hostname+"/spoken_tutorial_org/forum/";
	loading_image = "<img src='http://"+location.hostname+"/spoken_tutorial_org/ajax-loader.gif' />";
	// reset form item
	$('.reset_item').css({'cursor':'pointer','color':'green'});
	$('.reset_item').click(function(){
		var cls = $(this).attr('class');
		var c=cls.split(" "); 
		//c[1]
		$('.item'+c[1]+'_location').attr("checked", false);
		$('.prvideo_min'+c[1]).val('');
		$('.prvideo_sec'+c[1]).val('');
	});
	
	// addmore option
	$('.addmore').click(function(){
		var cls = $(this).attr('class');
		var c=cls.split(" "); 
		//c[1]
		//alert(c[1]);
		// get how many row is showing
		var showed_item = $('.item_visible'+c[1]).val();
		//alert(showed_item);
		//alert('.'+c[1]+(parseInt(showed_item)+1));
		var showed_item = (parseInt(showed_item)+1)
		//show next item after click add more
		$('.'+c[1]+showed_item).css({'display':''});
		
		// add visible item
		$('.item_visible'+c[1]).val(showed_item);
	});
	//$('.pritems').css({'display':'none'});
	//$('.form-checkbox').attr("checked", false);
    //$('.form-radio').attr("disabled", true);
    //$('.form-radio').attr("checked", false);
    //$('.prvideo_min').attr("disabled", true);
    //$('.prvideo_sec').attr("disabled", true);
    
    /*
	$('#edit-item1').click(function(){
	    if($(this).attr('checked')){
	        $('.item1_location').attr("disabled", false);
		    $('#edit-item1prvideo-min').attr("disabled", false);
            $('#edit-item1prvideo-sec').attr("disabled", false);
            $('.item1_location').attr("checked", true);
	    }else{
	        $('.item1_location').attr("disabled", true);
		    $('#edit-item1prvideo-min').attr("disabled", true);
            $('#edit-item1prvideo-sec').attr("disabled", true);
            $('.item1_location').attr("checked", false);
	    }
    });
    
    $('#edit-item2').click(function(){
        if($(this).attr('checked')){
            $('.item2_location').attr("disabled", false);
		    $('#edit-item2prvideo-min').attr("disabled", false);
            $('#edit-item2prvideo-sec').attr("disabled", false);
            $('.item2_location').attr("checked", true);
        }else{
            $('.item2_location').attr("disabled", true);
		    $('#edit-item2prvideo-min').attr("disabled", true);
            $('#edit-item2prvideo-sec').attr("disabled", true);
            $('.item2_location').attr("checked", false);
        }
    });
    $('#edit-item3').click(function(){
        if($(this).attr('checked')){
            $('.item3_location').attr("disabled", false);
	        $('#edit-item3prvideo-min').attr("disabled", false);
            $('#edit-item3prvideo-sec').attr("disabled", false);
            $('.item3_location').attr("checked", true);
        }else{
            $('.item3_location').attr("disabled", true);
	        $('#edit-item3prvideo-min').attr("disabled", true);
            $('#edit-item3prvideo-sec').attr("disabled", true);
            $('.item3_location').attr("checked", false);
        }
    });
    $('#edit-item4').click(function(){
        if($(this).attr('checked')){
            $('.item4_location').attr("disabled", false);
	        $('#edit-item4prvideo-min').attr("disabled", false);
            $('#edit-item4prvideo-sec').attr("disabled", false);
            $('.item4_location').attr("checked", true);
        }else{
            $('.item4_location').attr("disabled", true);
	        $('#edit-item4prvideo-min').attr("disabled", true);
            $('#edit-item4prvideo-sec').attr("disabled", true);
            $('.item4_location').attr("checked", false);
        }
    });
    $('#edit-item5').click(function(){
        if($(this).attr('checked')){
            $('.item5_location').attr("disabled", false);
	        $('#edit-item5prvideo-min').attr("disabled", false);
            $('#edit-item5prvideo-sec').attr("disabled", false);
            $('.item5_location').attr("checked", true);
        }else{
            $('.item5_location').attr("disabled", true);
	        $('#edit-item5prvideo-min').attr("disabled", true);
            $('#edit-item5prvideo-sec').attr("disabled", true);
            $('.item5_location').attr("checked", false);
        }
    }); */
	$('.rptutorial-resource').change(function(){
	    
		if($(this).val() == 'video'){
			$('.pritems').css({'display':'block'});
		}else{
		   $('.form-checkbox').attr("checked", false);
		   $('.form-radio').attr("disabled", true);
		   $('.form-radio').attr("checked", false);
		   $('.prvideo_min').attr("disabled", true);
		   $('.prvideo_sec').attr("disabled", true);
		   $('.pritems').css({'display':'none'});
		}
	});
});

function validate_report(){
	if(!($('#edit-item9-result-0')).attr('checked')){
		if($('#edit-missing-comment').val() == ''){
			alert('Please give valid comment');
			return false;
		}
	}
}

