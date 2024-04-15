// Customize Jquery Plugins
$(document).ready(function() {
$("#navigation").sticky({topSpacing:0});
// Fancybox Execute
	$(".fancybox").fancybox({
		'transitionIn'	:	'fade',
		'transitionOut'	:	'fade',
		'speedIn'		:	500, 
		'speedOut'		:	500,
		'overlayShow'	:	true
	});
	$('.flexslider').flexslider({
        animation: "slide", 			 //String: Select your animation type, "fade" or "slide"
        slideshow: true,                //Boolean: Animate slider automatically
		slideshowSpeed: 5000,           //Integer: Set the speed of the slideshow cycling, in milliseconds
		animationSpeed: 600,
		controlNav: false,               //Boolean: Create navigation for paging control of each clide? Note: Leave true for manualControls usage
		directionNav: true,             //Boolean: Create navigation for previous/next navigation? (true/false)
		prevText: "Previous",           //String: Set the text for the "previous" directionNav item
		nextText: "Next",
		touch: true,
        start: function(slider){
          $('body').removeClass('loading');
        }
    });
    //Tinynav plugin execute
	$(function () {
	    $("#tinynav").tinyNav();
	});
	// Animated Scroll
	$("#home-link").anchorScroll({fx: "easeOutExpo"});
	$("#services-link").anchorScroll({fx: "easeOutExpo"});
	$("#works-link").anchorScroll({fx: "easeOutExpo"});
	$("#news-link").anchorScroll({fx: "easeOutExpo"});
	$("#team-link").anchorScroll({fx: "easeOutExpo"});
	$("#contact-link").anchorScroll({fx: "easeOutExpo"});
	// Works - Quicksand

  // get the action filter option item on page load
  var $filterType = $('#filterOptions li.active a').attr('class');
	
  // get and assign the ourHolder element to the
	// $holder varible for use later
  var $holder = $('ul.ourHolder');

  // clone all items within the pre-assigned $holder element
  var $data = $holder.clone();

  // attempt to call Quicksand when a filter option
	// item is clicked
	$('#filterOptions li a').click(function(e) {
		// reset the active class on all the buttons
		$('#filterOptions li').removeClass('active');
		
		// assign the class of the clicked filter option
		// element to our $filterType variable
		var $filterType = $(this).attr('class');
		$(this).parent().addClass('active');
		
		if ($filterType == 'all') {
			// assign all li items to the $filteredData var when
			// the 'All' filter option is clicked
			var $filteredData = $data.find('li');
		} 
		else {
			// find all li elements that have our required $filterType
			// values for the data-type element
			var $filteredData = $data.find('li[data-type=' + $filterType + ']');
		}
		$holder.quicksand($filteredData, {
			duration: 800,
			easing: 'easeInOutQuad'
		},function() { // callback function
		    $(".fancybox").fancybox();
		});
		return false;
	});
});
