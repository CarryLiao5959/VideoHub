from flask import Flask, jsonify, abort
from flask_cors import CORS,cross_origin
import json

app = Flask(__name__)
CORS(app)

FILE_PATH = '/home/engage/github_projects/socket/backend/static/movie1/barrages.json'

@app.route('/barrages', methods=['GET'])
@cross_origin()
def manage_barrages():
    try:
        with open(FILE_PATH, 'r') as file:
            data = json.load(file)
            return jsonify(data)
    except FileNotFoundError:
        abort(404, description="Barrages file not found")
    except json.JSONDecodeError:
        abort(500, description="Error decoding the barrages data")

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5001,debug=True)

