#pragma once

#include <functional>
#include <set>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> websocket_server;
typedef websocketpp::server<websocketpp::config::asio_tls> websocket_tls_server;
typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::lib::error_code error_code;

class WebSocket {
public:
  WebSocket();
  ~WebSocket();

  void on_http(connection_hdl hdl);
  void on_open(connection_hdl hdl);
  void on_close(connection_hdl hdl);
  void on_error(connection_hdl hdl,const error_code& ec);
  void on_message(connection_hdl hdl, websocket_server::message_ptr msg); 

  void run_ws_server(uint16_t port);

  void broadcast(const std::string& message);
  int get_vid_from_hdl(websocketpp::connection_hdl hdl);

private:
  websocket_server ws_server;
  // websocket_tls_server ws_server;
  std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections; 
};