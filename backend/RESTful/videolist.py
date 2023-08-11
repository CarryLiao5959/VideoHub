from flask import Flask, jsonify, abort,send_from_directory
import json

app = Flask(__name__, static_folder='/home/engage/github_projects/socket/backend/static')


@app.route('/videos', methods=['GET'])

def get_videos():
    try:
        with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
            data = json.load(file)
            return jsonify(data)
    except FileNotFoundError:
        abort(404,description="video list file not found")
    except json.JSONDecodeError:
        abort(500,description="Error decoding file")

@app.route('/videolist')
def video_list():
    frontend_path = "/home/engage/github_projects/socket/frontend"
    return send_from_directory(frontend_path, 'videolist_2.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000,debug=True)
