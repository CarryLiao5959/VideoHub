from flask import Flask, jsonify
import json

app = Flask(__name__)

@app.route('/videos', methods=['GET'])
def get_videos():
    with open('/home/engage/github_projects/socket/backend/RESTful/videos.json', 'r') as file:
        data = json.load(file)
        return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000,debug=True)
