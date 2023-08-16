from flask import Flask, jsonify, abort,send_from_directory,render_template
from flask_cors import CORS,cross_origin
from flask import request
import json

app = Flask(__name__, static_folder='/home/engage/github_projects/socket/backend/static')
CORS(app)

@app.route('/videos', methods=['GET'])
@cross_origin()
def get_videos():
    try:
        with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
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


# def get_video_url_by_tag(tag):
#     with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
#         data = json.load(file)
#         for video in data["videos"]:
#             if video["tag"] == tag:
#                 return video["url"]
#     return None


# @app.route('/videolist')
# @cross_origin()
# def video_list_tag():
#     tag = request.args.get('tag')
#     video_url = get_video_url_by_vid(tag)
#     if video_url is None:
#         abort(404, description="Video not found")
#     return render_template('video_page.html', video_url=video_url,tag=tag)


@app.route('/')
@cross_origin()
def video_list():
    return render_template('videolist.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=80,debug=True)
