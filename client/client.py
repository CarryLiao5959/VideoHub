import socket
import struct


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

    message = 'hello'
    data = message.encode()
    sent_bytes = client.send(data)
    if sent_bytes == len(data):
        print("Data was sent successfully!")
    else:
        print("Only part of data was sent!")

    buffer_size = 1024
    data = client.recv(buffer_size)
    if len(data) == 0:
        print("No data received, the connection might be closed.")
    elif len(data) > 0:
        print("Data received: ", data.decode())

    client.close()