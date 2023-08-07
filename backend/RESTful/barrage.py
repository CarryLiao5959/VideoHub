from flask import Flask, jsonify
import json

app = Flask(__name__)

@app.route('/barrages', methods=['GET'])
def get_barrages():
    with open('/home/engage/github_projects/socket/backend/RESTful/barrages.json', 'r') as file:
        data = json.load(file)
        return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5001,debug=True)
