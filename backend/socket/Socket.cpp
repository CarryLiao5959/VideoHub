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
        debug("set bind ip: %s", ip.c_str());
    }
    else
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        debug("set bind ip: INADDR_ANY");
    }
    sockaddr.sin_port = htons(port);
    if (::bind(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) < 0)
    {
        log_error("socket bind error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    info("bind success: %s", ip.c_str());
    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd, backlog) < 0)
    {
        printf("socket listen error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    debug("sockfd %d listen: %s:%d",m_sockfd,m_ip.c_str(),m_port);
    return true;
}

int Socket::accept()
{
    int sockfd = ::accept(m_sockfd, nullptr, nullptr);
    if (sockfd < 0)
    {
        sockfd = -1;
    }
    debug("accept success sockfd: %d", sockfd);
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

int Socket::recv(char *buf, int len,int flags=0) {
    return ::recv(m_sockfd, buf, len, flags);
}

int Socket::send(char *buf, int len) {
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::send(int *buf, int len) {
    return ::send(m_sockfd, buf, len, 0);
}

ssize_t Socket::readn(void*buf, size_t len, bool isBlock, bool isRead)
{
    // 这里将 void* 转换成 char* 是为了在下面进行自增操作
    char *pos = (char*)buf;
    size_t leftNum = len;
    ssize_t readNum = 0;
    while(leftNum > 0)
    {
        ssize_t tmpRead = 0;
        // 尝试循环读取,如果报错,则进行判断
        // 注意, read 的返回值为0则表示读取到 EOF,是正常现象
        if(isRead)
            tmpRead = recv(pos, leftNum);
        else
            tmpRead = recv(pos, leftNum, (isBlock ? 0 : MSG_DONTWAIT));
        
        if(tmpRead < 0)
        {
            if(errno == EINTR)
                tmpRead = 0;
            // 如果始终读取不到数据,则提前返回,因为这个取决于远程 fd,无法预测要等多久
            else if (errno == EAGAIN)
                return readNum;
            else
                return -1;
        }
        // 读取的0,则说明远程连接已被关闭
        if(tmpRead == 0)
            break;
        readNum += tmpRead;
        pos += tmpRead;

        // 如果是阻塞模式下,并且读取到的数据较小,则说明数据已经全部读取完成,直接返回
        if(isBlock && static_cast<size_t>(tmpRead) < leftNum)
            break;

        leftNum -= tmpRead;
    }
    return readNum;
}

ssize_t writen(int fd, void*buf, size_t len, bool isWrite)
{
    // 这里将 void* 转换成 char* 是为了在下面进行自增操作
    char *pos = (char*)buf;
    size_t leftNum = len;
    ssize_t writtenNum = 0;
    while(leftNum > 0)
    {
        ssize_t tmpWrite = 0;

        if(isWrite)
            tmpWrite = this->send(pos, leftNum);
        else
            tmpWrite = send(fd, pos, leftNum, 0);

        // 尝试循环写入,如果报错,则进行判断
        if(tmpWrite < 0)
        {
            // 与read不同的是,如果 EAGAIN,则继续重复写入,因为写入操作是有Server这边决定的
            if(errno == EINTR || errno == EAGAIN)
                tmpWrite = 0;
            else
                return -1;
        }
        if(tmpWrite == 0)
            break;
        writtenNum += tmpWrite;
        pos += tmpWrite;
        leftNum -= tmpWrite;
    }
    return writtenNum;
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