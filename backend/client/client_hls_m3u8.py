import socket
import struct
import time
import logging

class MsgHead:
    def __init__(self, flag, cmd, len):
        self.flag = flag
        self.cmd = cmd
        self.len = len

    def to_bytes(self):
        # assuming that flag is a string of 8 characters, cmd and len are unsigned 32-bit integers
        return struct.pack('8sII', self.flag.encode(), self.cmd, self.len)

if __name__ == '__main__':
    # Create a logger
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)

    # Create a file handler
    handler = logging.FileHandler('./log/m3u8.log')
    handler.setLevel(logging.INFO)

    # Create a logging format
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)

    # Add the handlers to the logger
    logger.addHandler(handler)

    host = '127.0.0.1'
    port = 8080
    client = socket.socket()
    print("client fd:", client.fileno(), "connect to", host, ":", port, "...")
    logger.info("client fd: %s connect to %s:%s ...", client.fileno(), host, port)

    try:
        client.connect((host, port))
    except socket.error as e:
        # print("Connection failed!")
        logger.info("Connection failed!")
    else:
        # print("Connection success!")
        logger.info("Connection success!")

    msg_flag = 'm3u8'
    msg = MsgHead(msg_flag, 6, 0)

    # Send header
    sent_bytes = client.send(msg.to_bytes())
    if sent_bytes == 16: 
        # print("Header sent success")
        logger.info("Header sent success")
    else:
        # print("Header sent fail!")
        logger.error("Header sent fail!")
        client.close()
        exit(1)

    buffer_size = 1024*4

    cnt = 1
    with open("./src/received.m3u8", "wb") as f:
        while True:
            data = client.recv(buffer_size)
            if not data: 
                break
            f.write(data)
            print("write package "+ str(cnt) +" len: " +str(len(data)))
            logger.info("write package %s len: %s", str(cnt) ,str(len(data)))
            if len(data) < buffer_size:
                break
            cnt += 1
        logger.info("File received success")

    time.sleep(1)

    client.close()