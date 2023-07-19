#include "ClientSocket.h"
using namespace yazi::socket;

#include <cstring>
using namespace std;

ClientSocket::ClientSocket() : Socket()
{
}

ClientSocket::ClientSocket(const string &ip, int port) : Socket(ip, port)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        return;
    }
    Socket::connect(ip, port);
}

ClientSocket::~ClientSocket()
{
    close();
}
