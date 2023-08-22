from websocket import create_connection
import websocket
import ssl

websocket.enableTrace(True)
ws = create_connection("wss://localhost:8081")
ws.send("Hello, Server")
result = ws.recv()
print(result)
ws.close()