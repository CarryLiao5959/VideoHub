#pragma once

#include <string>
using namespace std;

#include "Socket.h"

namespace yazi {
namespace socket {
class ClientSocket : public Socket {
  public:
    ClientSocket() : Socket() {}

    ClientSocket(const string &ip, int port) : Socket(ip, port) {
        m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_sockfd < 0) {
            return;
        }
        debug("new ClientSocket sockfd: %d", m_sockfd);
        while(1){
            bool ret=Socket::connect(ip, port);
            if(ret){
                debug("ClientSocket connect success: %d", m_sockfd);
                break;
            }
            warn("ClientSocket connect fail: %d", m_sockfd);
            sleep(1);
        }
    }

    virtual ~ClientSocket() { close(); }
};
} // namespace socket
} // namespace yazi