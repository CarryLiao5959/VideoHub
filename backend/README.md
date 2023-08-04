## 环境配置

wsl2

```bash
> wsl --list --verbose
  NAME      STATE           VERSION
* Ubuntu    Running         2
```

g++ & python3

```bash
$ g++ --version
g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

$ python3 --version
Python 3.8.10
```

## 文件编译

```bash
$ make
$ ./main
```

## 客户端连接

另起一个终端执行，客户端将下载服务端的文件：

```bash
$ python3 client/client_txt.py
$ python3 client/client_img.py
$ python3 client/client_gif.py
```

hls相关文件传输：

```bash
$ python3 client/client_m3u8.py          # 传输.m3u8文件
$ python3 client/client_ts.py            # 传输.ts文件
$ python3 client/client_ts_by_s.py 100   # 传输100秒附近的.ts文件
```

高并发测试：

```bash
$ python3 client/test_echo.py   #测试client_echo.py
$ python3 client/test_m3u8.py   #测试client_m3u8.py
```

## server IP配置

可以在config/main.ini文件中修改配置：

```ini
[server]
ip = 127.0.0.1  # 本地回环
ip =            # 设为空：任何地址可连接到服务端
port = 8080
```

## client并发数配置

可以在config/main.ini文件中修改配置：

```ini
[client]
threads = 100
```