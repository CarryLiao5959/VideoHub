from flask import Flask, jsonify, abort,render_template
from flask_cors import CORS,cross_origin
from flask import request
import json
import os
import logging
from werkzeug.utils import secure_filename
import re
import subprocess


app = Flask(__name__, static_folder='/home/engage/github_projects/socket/backend/static')
CORS(app)

UPLOAD_FOLDER = '/home/engage/github_projects/socket/backend/static/' 
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

ALLOWED_EXTENSIONS = {'mp4', 'avi', 'mov', 'flv'} 

logging.basicConfig(level=logging.DEBUG)

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


@app.route('/hello')
@cross_origin()
def hello():
    return "hello"


# 检查文件是否允许上传
def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


def path_is_valid(path):
    return os.path.exists(path) and os.access(path, os.W_OK)


def get_next_movie_folder(base_path):
    folders = [f for f in os.listdir(base_path) if os.path.isdir(os.path.join(base_path, f)) and re.match(r'movie\d+', f)]
    if not folders:
        next_movie_num = 1
    else:
        max_movie_num = max([int(re.search(r'\d+', folder).group(0)) for folder in folders])
        next_movie_num = max_movie_num + 1
    
    next_movie_folder = os.path.join(base_path, f"movie{next_movie_num}")
    os.makedirs(next_movie_folder)
    return next_movie_folder


def update_videos_json(movie_folder_name, mp4_filename):
    with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
        data = json.load(file)

    vid = str(len(data['videos']) + 1)  # 根据已有的视频数量增加1
    new_entry = {
        "title": os.path.splitext(mp4_filename)[0],  # 从.mp4文件名中移除扩展名
        "url": f"static/{movie_folder_name}/output.m3u8",
        "cover": f"static/{movie_folder_name}/cover.png",
        "vid": vid
    }

    data['videos'].append(new_entry)
    
    with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'w') as file:
        json.dump(data, file, indent=4)


def extract_frame(video_path, frame_number, output_path):
    cmd = [
        'ffmpeg', 
        '-i', video_path, 
        '-vf', f'select=eq(n\,{frame_number})', 
        '-vframes', '1', 
        output_path
    ]
    subprocess.check_call(cmd)


@app.route('/upload', methods=['POST'])
@cross_origin()
def upload_file():
    logging.debug("upload_file")
    try:
        if 'file' not in request.files:
            return jsonify({"error": "No file part"}), 400
        file = request.files['file']
        if file.filename == '':
            return jsonify({"error": "No selected file"}), 400
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)

            # 获取新的文件夹路径并创建
            next_movie_folder = get_next_movie_folder(app.config['UPLOAD_FOLDER'])
            filepath = os.path.join(next_movie_folder, filename)

            # Check if path is valid
            if not path_is_valid(app.config['UPLOAD_FOLDER']):
                logging.error("Invalid or non-writable upload path.")
                return jsonify({"error": "Server error: Invalid upload path"}), 500
            logging.info(f"filepath: {filepath}")

            try:
                file.save(filepath)
                logging.info(f"File successfully saved to: {filepath}")

                # Extract cover image from uploaded video
                cover_path = os.path.join(next_movie_folder, 'cover.png')
                try:
                    extract_frame(filepath, 25, cover_path)  # 25 is the frame number; you can adjust it as needed
                    logging.info(f"Cover image extracted to: {cover_path}")
                except Exception as e:
                    logging.error(f"Error extracting cover image: {e}")

                # 创建 barrages.json
                barrages_path = os.path.join(next_movie_folder, 'barrages.json')
                try:
                    with open(barrages_path, 'w') as f:
                        json.dump({}, f)
                    logging.info(f"barrages.json successfully created at: {barrages_path}")
                except Exception as e:
                    logging.error(f"Error creating barrages.json: {e}")

                subprocess_cmd = ['sudo','/home/engage/github_projects/socket/backend/config/ffmpeg.sh', os.path.basename(next_movie_folder), '1']
                try:
                    subprocess.check_call(subprocess_cmd)
                    logging.info(f"Successfully executed: {' '.join(subprocess_cmd)}")
                except subprocess.CalledProcessError as e:
                    logging.error(f"Error executing: {' '.join(subprocess_cmd)}")
                    logging.error(e)

                update_videos_json(os.path.basename(next_movie_folder), filename)

            except Exception as e:
                logging.error(f"Error saving file to {filepath}: {str(e)}")
                raise e  # 重新抛出异常以供上级处理或让其终止程序

            return jsonify({"message": "File uploaded successfully", "filepath": filepath}), 200
        else:
            return jsonify({"error": "File type not allowed"}), 400
    except Exception as e:
        logging.error(f"Error uploading file: {e}")
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=80,debug=True)
