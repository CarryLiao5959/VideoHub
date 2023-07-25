#include "Server.h"
using namespace yazi::server;
#include "Singleton.h"
#include "Logger.h"
using namespace yazi::util;
#include "TaskDispatcher.h"
#include "AutoLock.h"
using namespace yazi::thread;
#include "SocketHandler.h"
#include "ClientSocket.h"
using namespace yazi::socket;

Server::Server() : m_ip(""), m_port(0), m_threads(1024), m_connects(1024), m_wait_time(10){};

Server::~Server(){};

void Server::listen(const string &ip, int port)
{
    m_ip = ip;
    m_port = port;
    debug("listen: %s:%d", m_ip.c_str(), m_port);
}

void Server::start()
{
    // init the thread pool and task queue
    TaskDispatcher *dispatcher = Singleton<TaskDispatcher>::instance();
    dispatcher->init(m_threads);

    // init the socket handler
    SocketHandler *socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(m_ip, m_port);
    socket_handler->handle(m_connects, m_wait_time);
}

void Server::set_threads(int threads) { 
    m_threads = threads;
    debug("set_threads: %d", m_threads);
}

void Server::set_connects(int connects) { 
    m_connects = connects; 
    debug("set_connects: %d", m_connects); 
}

void Server::set_wait_time(int wait_time) {
    m_wait_time = wait_time;
    debug("set_wait_time: %d", m_wait_time); 
}