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
    print("client fd:", client.fileno(), "connect to", host, ":", port, "...")

    try:
        client.connect((host, port))
    except socket.error as e:
        print("Connection failed!")
    else:
        print("Connection success!")

    message = 'hello server'
    msg_flag = 'ECHO'
    msg = MsgHead(msg_flag, 1, len(message))

    # Send header
    sent_bytes = client.send(msg.to_bytes())
    if sent_bytes == 16: 
        print("Header sent success")
    else:
        print("Header sent fail!")
        client.close()
        exit(1)

    buffer_size = 1024
    # Wait for 'OK' from server
    # data = client.recv(buffer_size)  # receive data from the server
    # if data.decode().strip() != 'OK':
    #     print("'OK' not received!")
    #     client.close()
    #     exit(1)
    # else:
    #     print("'OK' received.")

    # Send data
    data = message.encode()
    sent_bytes = client.send(data)
    if sent_bytes == len(data):
        print("Data sent success")
    else:
        print("Data sent fail!")

    data = client.recv(buffer_size)
    if len(data) == 0:
        print("data received fail!")
    elif len(data) > 0:
        print("Data received: ", data.decode())

    time.sleep(1)

    client.close()