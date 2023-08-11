#include "WebSocket.h"
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;
#include "JsonHandler.h"

WebSocket::WebSocket() {
    ws_server.init_asio();
    ws_server.set_message_handler(bind(&WebSocket::on_message, this, std::placeholders::_1, std::placeholders::_2));
    ws_server.set_open_handler(std::bind(&WebSocket::on_open, this, std::placeholders::_1));
    ws_server.set_close_handler(bind(&WebSocket::on_close, this, std::placeholders::_1));
}

WebSocket::~WebSocket() {
}

void WebSocket::on_open(connection_hdl hdl) {
    // debug("on_open");
    m_connections.insert(hdl);
}

void WebSocket::on_close(connection_hdl hdl) {
    // debug("on_close");
    m_connections.erase(hdl);
}

void WebSocket::broadcast(const std::string& message) {
    // debug("broadcast");
    for (auto hdl : m_connections) {
        ws_server.send(hdl, message, websocketpp::frame::opcode::text);
    }
}

void WebSocket::on_message(websocketpp::connection_hdl hdl, websocket_server::message_ptr msg) {
    // debug("on_message");

    string received = msg->get_payload();
    std::cout << "Received message: " << received << std::endl;
    info("Received message: %s", received.c_str());

    string filepath= "/home/engage/github_projects/socket/backend/static/movie1/barrages.json";
    Singleton<JsonHandler>::instance(filepath)->append_to_file(received);

    std::string response = "Hello from the WebSocket server!";
    ws_server.send(hdl, response, websocketpp::frame::opcode::text);

    std::string broadcast_info = "barrage updated!";
    broadcast(broadcast_info);
}

void WebSocket::run_ws_server(uint16_t port) {
    // debug("run_ws_server");
    ws_server.listen(port);
    ws_server.start_accept();
    ws_server.run();
}