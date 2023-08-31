from flask import Flask
from flask_cors import CORS

app = Flask(__name__, static_folder='../../static')
CORS(app)

# 确保路由在app创建后被加载
from videolist import routes