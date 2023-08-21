#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

namespace yazi {
namespace socket {
class Socket {
    friend class SocketHandler;

  public:
    Socket();
    Socket(const string &ip, int port);
    ~Socket();

    bool bind(const string &ip, int port);
    bool listen(int backlog);
    int accept();
    bool connect(const string &ip, int port);
    int recv(char *buf, int len);
    int recv(char *buf, int len,int flags);
    int send(char *buf, int len);
    int send(int *buf, int len);
    bool close();

    ssize_t readn(void*buf, size_t len, bool isBlock);
    ssize_t writen(void*buf, size_t len);

    bool set_non_blocking();
    bool set_send_buffer(int size);
    bool set_recv_buffer(int size);
    bool set_linger(bool active, int seconds);
    bool set_keep_alive();
    bool set_reuse_addr();
    bool set_reuse_port();

    int getfd() { return m_sockfd; }
    void setfd(int fd) { m_sockfd=fd; }

  protected:
    string m_ip;
    int m_port;
    int m_sockfd;
};
} // namespace socket
} // namespace yazi