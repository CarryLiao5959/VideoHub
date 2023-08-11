import socket
import struct
import time
import sys
import logging

import random

def time_to_seconds(time_str):
    h, m, s = time_str.split(':')
    s, ms = s.split('.')
    total_seconds = int(h) * 3600 + int(m) * 60 + int(s) + int(ms) / 100.0
    return total_seconds

def seconds_to_time(seconds):
    h = int(seconds // 3600)
    seconds %= 3600
    m = int(seconds // 60)
    seconds %= 60
    s = int(seconds)
    ms = int((seconds - s) * 1000)
    return "{:02}:{:02}:{:02}.{:03}".format(h, m, s, ms)

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
    handler = logging.FileHandler('/home/engage/github_projects/socket/backend/client/log/barrage.log')
    handler.setLevel(logging.INFO)

    # Create a logging format
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)

    # Add the handlers to the logger
    logger.addHandler(handler)

    # sys.stdout = open('./log/output.log', 'w')  # redirect print output to file

    host = '127.0.0.1'
    port = 8080
    client = socket.socket()
    # client.settimeout(30.0)
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

    video_duration_str = "01:30:02.050"
    video_duration_seconds = time_to_seconds(video_duration_str)
    random_seconds = random.uniform(0, video_duration_seconds)
    random_timestamp = seconds_to_time(random_seconds)

    message = random_timestamp + '-' + 'this movie is so good!'
    msg_flag = 'Barrage'
    msg = MsgHead(msg_flag, 9, len(message))

    # Send header
    sent_bytes = client.send(msg.to_bytes())
    if sent_bytes == 16: 
        # print("Header sent success")
        logger.info("Header sent success")
    else:
        # print("Header sent fail!")
        logger.info("Header sent fail!")
        client.close()
        exit(1)

    buffer_size = 1024

    # Send data
    data = message.encode()
    sent_bytes = client.send(data)
    if sent_bytes == len(data):
        # print("Data sent success")
        logger.info("Data sent success")
    else:
        # print("Data sent fail!")
        logger.info("Data sent fail!")

    while True:
        try:
            data = client.recv(buffer_size)
        except ConnectionResetError:
            # print("Connection was reset by the server.")
            logger.info("Connection was reset by the server.")
            client.close()
            exit(1)
        else:
            if len(data) == 0:
                # print("data received fail!")
                logger.info("data received fail!")
            elif len(data) > 0:
                print("Data received: ", data.decode())
                logger.info("Data received: %s", data.decode())

    time.sleep(1)

    client.close()