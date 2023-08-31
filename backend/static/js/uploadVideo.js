// 当点击按钮时触发文件选择框
document.querySelector('.btn-primary').addEventListener('click', function() {
    document.getElementById('videoUpload').click();
});

// 当选择了视频文件后，处理上传逻辑
document.getElementById('videoUpload').addEventListener('change', function(event) {
    const selectedVideo = event.target.files[0];
    if (selectedVideo) {
        // 创建一个 FormData 对象来容纳要上传的文件
        const formData = new FormData();
        formData.append('file', selectedVideo);

        // 使用 fetch API 上传文件到 Flask 服务器
        fetch('/upload', {
            method: 'POST',
            body: formData
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            if (data.message) {
                alert(data.message); // 或任何其他的成功提示
            } else if (data.error) {
                alert(data.error);  // 或任何其他的错误提示
            }
        })
        .catch(error => {
            console.error('Error uploading file:', error);
            alert('An error occurred while uploading. Please try again.'); // 或任何其他的错误提示
        });
    }
});