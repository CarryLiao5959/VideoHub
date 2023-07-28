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

## server IP配置

可以在config/main.ini文件中修改配置：

```ini
[server]
ip = 127.0.0.1
port = 8080
```