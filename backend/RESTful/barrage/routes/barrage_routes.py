from flask import jsonify, abort, request
from barrage import app
from flask_cors import cross_origin
import json

BASE_PATH = '/home/engage/github_projects/socket/backend/static/movie'

@app.route('/barrage', methods=['GET'])
@cross_origin()
def manage_barrages():
    # 获取查询参数vid的值
    vid = request.args.get('vid')

    # 确保vid是一个数字
    if not vid or not vid.isdigit():
        abort(400, description="Invalid vid parameter")

    file_path = f"{BASE_PATH}{vid}/barrages.json"

    try:
        with open(file_path, 'r') as file:
            data = json.load(file)
            return jsonify(data)
    except FileNotFoundError:
        abort(404, description=f"Barrages file for movie{vid} not found")
    except json.JSONDecodeError:
        abort(500, description="Error decoding the barrages data")