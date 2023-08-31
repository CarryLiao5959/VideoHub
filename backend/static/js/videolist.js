$(document).ready(function () {
    // 保存navbar的初始位置
    const navbarOffsetTop = $(".navbar").offset().top;

    $(window).on('scroll', function () {
        if ($(window).scrollTop() > navbarOffsetTop) {
            $('.navbar').addClass('fixed-top');
        } else {
            $('.navbar').removeClass('fixed-top');
        }
    });
    // Function to load videos based on tab
    function loadVideos(tab = 'videos') {
        $.ajax({
            url: `https://engage-2023.com/videos?tab=${tab}`, // the backend supports the 'tab' parameter
            method: 'GET',
            success: function (data) {
                $('#videolist').empty();  // Clear the existing videos
                data.videos.forEach(function (video) {
                    let videoItem = `
                            <li class="video video_item">
                                <a href="https://engage-2023.com/video?vid=${video.vid}">
                                    <div class="video_cover_container">
                                        <img src="${video.cover}" alt="${video.title}" class="video_cover">
                                        // <video poster="${video.cover}" preload="none" class="video_cover"></video>
                                    </div>
                                    <div class="video_title">${video.title}</div>
                                </a>
                            </li>
                        `;
                    $('#videolist').append(videoItem);
                    // Check if the video is in .m3u8 format and if hls.js is supported
                    // if (Hls.isSupported() && video.url.endsWith('.m3u8')) {
                    //     let videoElement = $('.video_cover').last().get(0);
                    //     let hls = new Hls();
                    //     hls.loadSource(video.url);
                    //     hls.attachMedia(videoElement);
                    // } else {
                    //     $('.video_cover').last().attr('src', video.url);
                    // }
                });
                // Handle mouse enter and leave events for the video tags
                // $('.video_cover').on('mouseenter', function() {
                //     $(this).get(0).play();
                // });
                
                // $('.video_cover').on('mouseleave', function() {
                //     let videoElement = $(this).get(0);
                //     videoElement.pause();
                //     videoElement.currentTime = 0; // reset video to start
                // });
            },
            error: function (error) {
                alert('Error fetching videos: ' + error.statusText);
            }
        });
    }

    // Load videos for default tab 'A' on page load
    loadVideos();

    // Handle tab clicks
    $('.nav-item').on('click', function () {
        $('.nav-item').removeClass('active');  // Remove 'active' class from all tabs
        $(this).addClass('active');            // Add 'active' class to clicked tab
        let tab = $(this).attr('id');  // Extract tab name from id
        loadVideos(tab);
    });
});