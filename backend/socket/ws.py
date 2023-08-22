import json
import ssl
from urllib.parse import urlparse, parse_qs
from websocket_server import WebsocketServer

class WebSocketServerHandler:
    def __init__(self):
        self.connections = set()

    def on_client_connected(self, client, server):
        self.connections.add(client)
        print(f"Client {client['id']} connected")

    def on_client_disconnected(self, client, server):
        self.connections.discard(client)
        print(f"Client {client['id']} disconnected")

    def on_message_received(self, client, server, message):
        print(f"Received message from client {client['id']}: {message}")
        
        # Extract vid from the client's path
        vid = self.get_vid_from_path(client['path'])
        if vid:
            filepath = f"/home/engage/github_projects/socket/backend/static/movie{vid}/barrages.json"
            self.append_to_file(filepath, message)

            # Send a response to the client
            server.send_message(client, "Hello from the WebSocket server!")
            
            # Broadcast a message to all clients
            broadcast_info = "barrage updated!"
            for conn in self.connections:
                server.send_message(conn, broadcast_info)

    def get_vid_from_path(self, path):
        # Extract vid from the query parameters
        query = urlparse(path).query
        query_params = parse_qs(query)
        vid = query_params.get('vid', [None])[0]
        return vid

    def append_to_file(self, filepath, message):
        with open(filepath, 'a') as file:
            json.dump(message, file)
            file.write("\n")

    def run(self, port):
        server = WebsocketServer("0.0.0.0",port)
        server.set_fn_new_client(self.on_client_connected)
        server.set_fn_client_left(self.on_client_disconnected)
        server.set_fn_message_received(self.on_message_received)
        # SSL specific configurations
        context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        context.load_cert_chain(certfile="/etc/nginx/sites-available/0000_cert.pem", 
                                keyfile="/etc/nginx/sites-available/engage.key")
        server.ssl = context
        server.run_forever()

if __name__ == "__main__":
    handler = WebSocketServerHandler()
    handler.run(8081)