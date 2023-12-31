import socket
import struct
import time

class MsgHead:
    def __init__(self, flag, cmd, len):
        self.flag = flag
        self.cmd = cmd
        self.len = len

    def to_bytes(self):
        # assuming that flag is a string of 8 characters, cmd and len are unsigned 32-bit integers
        return struct.pack('8sII', self.flag.encode(), self.cmd, self.len)

if __name__ == '__main__':

    host = '127.0.0.1'
    port = 8080
    client = socket.socket()
    client.settimeout(2.0)
    print("client fd:", client.fileno(), "connect to", host, ":", port, "...")

    try:
        client.connect((host, port))
    except socket.error as e:
        print("Connection failed!")
    else:
        print("Connection success!")

    msg_flag = 'MP4'
    msg = MsgHead(msg_flag, 5, 0)

    # Send header
    sent_bytes = client.send(msg.to_bytes())
    if sent_bytes == 16: 
        print("Header sent success")
    else:
        print("Header sent fail!")
        client.close()
        exit(1)

    buffer_size = 1024*8

    cnt = 1
    with open("./src/received.mp4", "wb") as f:
        while True:
            try:
                data = client.recv(buffer_size)
            except socket.timeout:
                print("Recv Timeout")
                break
            if not data: 
                break
            f.write(data)
            if cnt%100 == 0:
                print("write package "+ str(cnt) +" len: " +str(len(data)))
            cnt += 1
        print("write package "+ str(cnt) +" len: " +str(len(data)))
        print("File received success")

    time.sleep(1)

    client.close()