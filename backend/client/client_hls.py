import socket
import struct
import time

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
    
    buffer_size = 1024

    files_num = client.recv(buffer_size)
    print(f"Receiving files_num: " + files_num)

    # for _ in range(file_num):
    #     filename_len = struct.unpack('I', client.recv(4))[0]
    #     filename = client.recv(filename_len).decode()

    #     print(f"Receiving file {filename}...")

    #     with open(f"./src/{filename}", "wb") as f:
    #         while True:
    #             try:
    #                 data = client.recv(buffer_size)
    #             except socket.timeout:
    #                 print("Recv Timeout")
    #                 break
    #             if not data: 
    #                 break
    #             f.write(data)
    #         print("File received success")

    time.sleep(1)

    client.close()
