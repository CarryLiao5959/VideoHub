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

## TLS 配置

```bash
$ echo | openssl s_client -connect engage-2023.com:443
CONNECTED(00000003)
depth=2 C = US, O = Internet Security Research Group, CN = ISRG Root X1
verify return:1
depth=1 C = US, O = Let's Encrypt, CN = R3
verify return:1
depth=0 CN = engage-2023.com
verify return:1
---
Certificate chain
 0 s:CN = engage-2023.com
   i:C = US, O = Let's Encrypt, CN = R3
 1 s:C = US, O = Let's Encrypt, CN = R3
   i:C = US, O = Internet Security Research Group, CN = ISRG Root X1
 2 s:C = US, O = Internet Security Research Group, CN = ISRG Root X1
   i:O = Digital Signature Trust Co., CN = DST Root CA X3
---
Server certificate
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
subject=CN = engage-2023.com

issuer=C = US, O = Let's Encrypt, CN = R3

---
No client certificate CA names sent
Peer signing digest: SHA256
Peer signature type: ECDSA
Server Temp Key: X25519, 253 bits
---
SSL handshake has read 4129 bytes and written 387 bytes
Verification: OK
---
New, TLSv1.3, Cipher is TLS_AES_256_GCM_SHA384
Server public key is 256 bit
Secure Renegotiation IS NOT supported
Compression: NONE
Expansion: NONE
No ALPN negotiated
Early data was not sent
Verify return code: 0 (ok)
---
DONE
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