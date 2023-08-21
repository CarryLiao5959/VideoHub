#include "WebSocket.h"
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;
#include "JsonHandler.h"

WebSocket::WebSocket() {
    ws_server.init_asio();

    // ws_server.set_tls_init_handler([&](websocketpp::connection_hdl hdl) -> std::shared_ptr<boost::asio::ssl::context> {
    //     auto ctx = make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    //     ctx->set_options(boost::asio::ssl::context::default_workarounds |
    //                     boost::asio::ssl::context::no_sslv2 |
    //                     boost::asio::ssl::context::no_sslv3 |
    //                     boost::asio::ssl::context::single_dh_use);
    //     ctx->use_certificate_chain_file("/etc/apache2/sites-available/0001_chain.pem");
    //     ctx->use_certificate_file("/etc/apache2/sites-available/0000_cert.pem", boost::asio::ssl::context::pem);
    //     ctx->use_private_key_file("/etc/apache2/sites-available/engage.key", boost::asio::ssl::context::pem);
    //     return ctx;
    // });
    // 配置跨域连接
    ws_server.set_http_handler(bind(&WebSocket::on_http, this, std::placeholders::_1));

    ws_server.set_message_handler(bind(&WebSocket::on_message, this, std::placeholders::_1, std::placeholders::_2));
    ws_server.set_open_handler(std::bind(&WebSocket::on_open, this, std::placeholders::_1));
    ws_server.set_close_handler(bind(&WebSocket::on_close, this, std::placeholders::_1));
}

WebSocket::~WebSocket() {
}

void WebSocket::on_http(websocketpp::connection_hdl hdl) {
    auto conn = ws_server.get_con_from_hdl(hdl);
    conn->append_header("Access-Control-Allow-Origin", "*");
}

void WebSocket::on_open(connection_hdl hdl) {
    debug("on_open");
    m_connections.insert(hdl);
}

void WebSocket::on_close(connection_hdl hdl) {
    debug("on_close");
    m_connections.erase(hdl);
}

void WebSocket::broadcast(const std::string& message) {
    debug("broadcast");
    for (auto hdl : m_connections) {
        ws_server.send(hdl, message, websocketpp::frame::opcode::text);
    }
}

void WebSocket::on_message(websocketpp::connection_hdl hdl, websocket_server::message_ptr msg) {
    debug("on_message");

    string received = msg->get_payload();
    std::cout << "Received message: " << received << std::endl;
    info("Received message: %s", received.c_str());

    int vid = get_vid_from_hdl(hdl); 
    string filepath = "/home/engage/github_projects/socket/backend/static/movie" + std::to_string(vid) + "/barrages.json";
    Singleton<JsonHandler>::instance(filepath)->append_to_file(received);

    std::string response = "Hello from the WebSocket server!";
    ws_server.send(hdl, response, websocketpp::frame::opcode::text);

    std::string broadcast_info = "barrage updated!";
    broadcast(broadcast_info);
}

int WebSocket::get_vid_from_hdl(websocketpp::connection_hdl hdl) {
    websocket_server::connection_ptr con = ws_server.get_con_from_hdl(hdl);
    std::string uri = con->get_resource(); // 获取URI，例如 "/barrage?vid=8"
    
    // 进行字符串解析，从URI中提取vid的值
    size_t pos = uri.find("vid=");
    if (pos == std::string::npos) {
        return -1; // 返回-1或其他错误值表示没有找到vid参数
    }
    pos += 4; // 跳过"vid="字符
    size_t end_pos = uri.find("&", pos); // 查找其他参数的开始，如果有的话
    std::string vid_str = (end_pos == std::string::npos) ? uri.substr(pos) : uri.substr(pos, end_pos - pos);
    
    return std::stoi(vid_str); // 将字符串转换为整数并返回
}

void WebSocket::run_ws_server(uint16_t port) {
    debug("run_ws_server");
    ws_server.listen(boost::asio::ip::tcp::v4(),port);
    ws_server.start_accept();
    ws_server.run();
}