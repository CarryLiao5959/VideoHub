from flask import Flask, jsonify, abort,render_template
from flask_cors import CORS,cross_origin
from flask import request
import json
import os
from werkzeug.utils import secure_filename

app = Flask(__name__, static_folder='/home/engage/github_projects/socket/backend/static')
CORS(app)

UPLOAD_FOLDER = '/home/engage/github_projects/socket/backend/static' 
ALLOWED_EXTENSIONS = {'mp4', 'avi', 'mov', 'flv'} 

@app.route('/videos', methods=['GET'])
@cross_origin()
def get_videos():
    tab = request.args.get('tab', default='videos')
    filename = tab + ".json"
    try:
        filepath = f'/home/engage/github_projects/socket/backend/RESTful/{filename}'
        with open(filepath, 'r') as file:
            data = json.load(file)
            return jsonify(data)
    except FileNotFoundError:
        abort(404,description="video list file not found")
    except json.JSONDecodeError:
        abort(500,description="Error decoding file")


def get_video_url_by_vid(vid):
    with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
        data = json.load(file)
        for video in data["videos"]:
            if video["vid"] == vid:
                return video["url"]
    return None


@app.route('/video')
@cross_origin()
def video_page():
    vid = request.args.get('vid')
    video_url = get_video_url_by_vid(vid)
    if video_url is None:
        abort(404, description="Video not found")
    return render_template('video_page.html', video_url=video_url,vid=vid)


@app.route('/')
@cross_origin()
def video_list():
    return render_template('videolist.html')

# 检查文件是否允许上传
def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


@app.route('/upload', methods=['POST'])
@cross_origin()
def upload_file():
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files['file']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400
    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(filepath)
        return jsonify({"message": "File uploaded successfully", "filepath": filepath}), 200
    else:
        return jsonify({"error": "File type not allowed"}), 400


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=80,debug=True)
