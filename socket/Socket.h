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

namespace yazi
{
    namespace socket
    {
        class Socket
        {
            friend class SocketHandler;

        public:
            Socket();
            Socket(const string &ip, int port);
            ~Socket();

            bool bind(const string &ip, int port);
            bool listen(int backlog);
            int accept();
            bool close();

            bool set_non_blocking();

        protected:
            string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}