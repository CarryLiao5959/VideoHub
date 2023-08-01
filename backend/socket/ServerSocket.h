#pragma once

#include "Socket.h"
#include <string>
#include "Logger.h"
using namespace yazi::util;
using namespace std;

namespace yazi {
namespace socket {
class ServerSocket : public Socket {
  public:
    ServerSocket() : Socket() {}

    ServerSocket(const string &ip, int port) : Socket(ip, port) {
        m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_sockfd == -1) {
            printf("create server socket error: errno=%d errstr=%s", errno,strerror(errno));
            return;
        }
        debug("new ServerSocket sockfd: %d", m_sockfd);

        set_non_blocking();
        set_recv_buffer(10 * 1024);
        set_send_buffer(10 * 1024);
        set_linger(true, 0);
        set_keep_alive();
        set_reuse_addr();

        bind(ip, port);
        listen(1024);
    }
    
    ~ServerSocket() {}
};
} // namespace socket
} // namespace yazi