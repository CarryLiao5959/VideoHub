## 文件编译

```bash
$ make
```

## 服务器进程

```bash
# WebSocket传输弹幕服务
$ cd backend/
$ ./main
# 视频列表服务
$ cd backend/RESTful/
$ python3 videolist.py
# 弹幕列表服务
$ cd /backend/RESTful/
$ python3 barrage.py
# 程序后台运行
$ sudo nohup python3 videolist.py > videolist.log 2>&1 &
$ sudo nohup python3 barrage.py > barrage.log 2>&1 &
```