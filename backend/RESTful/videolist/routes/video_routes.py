from flask import jsonify, abort, request, render_template
from flask_cors import cross_origin
from videolist import app
from videolist.utils.video_utils import get_video_url_by_vid
import json


@app.route('/videos', methods=['GET'])
@cross_origin()
def get_videos():
    tab = request.args.get('tab', default='videos')
    filename = tab + ".json"
    try:
        filepath = f'/home/engage/github_projects/socket/backend/RESTful/videolist/data/{filename}'
        with open(filepath, 'r') as file:
            data = json.load(file)
            return jsonify(data)
    except FileNotFoundError:
        abort(404,description="video list file not found")
    except json.JSONDecodeError:
        abort(500,description="Error decoding file")


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
