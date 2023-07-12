#include "Socket.h"

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