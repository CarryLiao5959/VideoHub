#include "Socket.h"
#include "Logger.h"
using namespace yazi::util;

using namespace yazi::socket;

Socket::Socket() {}
Socket::Socket(const string &ip, int port) : m_ip(ip), m_port(port), m_sockfd(0) {}
Socket::~Socket() {}

bool Socket::bind(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    if (ip != "")
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    else
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    sockaddr.sin_port = htons(port);
    if (::bind(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) < 0)
    {
        printf("socket bind error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd, backlog) < 0)
    {
        printf("socket listen error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

int Socket::accept()
{
    int sockfd = ::accept(m_sockfd, nullptr, nullptr);
    if (sockfd < 0)
    {
        sockfd = -1;
    }
    return sockfd;
}

bool Socket::connect(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        debug("connect failed");
        return false;
    }
    return true;
}

bool Socket::close()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
    return true;
}

bool Socket::set_non_blocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        return false;
    }
    flags |= O_NONBLOCK;
    if (fcntl(m_sockfd, F_SETFL, flags) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_linger(bool active, int seconds)
{
    struct linger l;
    memset(&l, 0, sizeof(l));

    if (active)
        l.l_onoff = 1;
    else
        l.l_onoff = 0;
    l.l_linger = seconds;

    if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_keep_alive()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_reuse_addr()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
    {
        return false;
    }
    return true;
}

bool Socket::set_reuse_port()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) < 0)
    {
        return false;
    }
    return true;
}