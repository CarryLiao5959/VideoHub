#include "SocketHandler.h"
#include "Logger.h"
#include "TaskFactory.h"
using namespace yazi::util;
using namespace yazi::socket;
using namespace yazi::task;
#include<algorithm>
#include <sys/inotify.h>

SocketHandler::SocketHandler() {}
SocketHandler::~SocketHandler() {}

void SocketHandler::listen(const string &ip, int port)
{
    m_server = new ServerSocket(ip, port);
}

void SocketHandler::handle(int max_connects, int wait_time)
{
    m_epoll = new EventPoller(false);
    m_epoll->create(max_connects);
    m_epoll->add(m_server->m_sockfd, m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
    m_sockpool.init(max_connects);

    int inotify_fd= inotify_init();
    if (inotify_fd < 0) {
        log_error("inotify_init");
    }
    debug("inotify_fd = %d", inotify_fd);
    string watched_file="/home/engage/github_projects/socket/backend/movie/movie1/barrages.json";
    int watch_fd= inotify_add_watch(inotify_fd, watched_file.c_str(), IN_MODIFY);
    if (watch_fd < 0) {
        log_error("inotify_add_watch");
    }
    m_epoll->add(inotify_fd, nullptr, EPOLLIN);
    debug("inotify_fd = %d", inotify_fd);

    while (1)
    {
        int num = m_epoll->wait(wait_time);
        if (num == 0)
        {
            // debug("epoll wait: find no events");
            continue;
        }
        debug("epoll wait: find %d events", num);
        for (int i = 0; i < num; i++)
        {
            // a new client try to connect -> server
            if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr))
            {
                int sockfd = m_server->accept();
                // 1 connect <-> 1 socket
                Socket *socket = m_sockpool.get();
                if (socket == nullptr)
                {
                    warn("no socket available in socketpool");
                    break;
                }
                socket->m_sockfd = sockfd;
                socket->set_non_blocking();
                attach(socket);
                active_sock.push_back(socket);
            } else if (0 == m_epoll->m_events[i].data.fd) {
                debug("m_epoll->m_events[i].data.fd = %d", m_epoll->m_events[i].data.fd);
                char buffer[1024];
                int length = read(inotify_fd, buffer, 1024);
                if (length < 0) {
                    log_error("read");
                }
                int index = 0;
                while (index < length) {
                    struct inotify_event *event = (struct inotify_event *)&buffer[index];
                    if (event->mask & IN_MODIFY) {
                        broadcast("barrages.json file was modified!");
                    }
                    index += sizeof(struct inotify_event) + event->len;
                }
            }else{
                // handle client event
                Socket *socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
                debug("m_epoll->m_events[i].data.fd = %d", m_epoll->m_events[i].data.fd);
                auto it=std::find(active_sock.begin(),active_sock.end(),socket);
                if (m_epoll->m_events[i].events & EPOLLHUP)
                {
                    debug("EPOLLHUP");
                    detach(socket);
                    if (it != active_sock.end()) {
                        active_sock.erase(it);
                    }
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLERR)
                {
                    debug("EPOLLERR");
                    detach(socket);
                    if (it != active_sock.end()) {
                        active_sock.erase(it);
                    }
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLIN)
                {
                    debug("EPOLLIN");
                    detach(socket);
                    Task * task = TaskFactory::create(socket);
                    Singleton<TaskDispatcher>::instance()->assign(task);
                }
            }
        }
    }
    inotify_rm_watch(inotify_fd, watch_fd);
    close(inotify_fd);
}

void SocketHandler::broadcast(const string& message) {
    debug("broadcast");
    if (message.size() > 31) {
        warn("Broadcast message is too long and will be truncated.");
    }
    char buf[32];
    memset(buf, 0, sizeof(buf));  // initialize buffer with zeros
    strncpy(buf, message.c_str(), sizeof(buf) - 1);  // copy up to 31 characters from message into buf
    for (Socket* socket : active_sock) {
        socket->send(buf, strlen(buf));  // send only the actual characters
    }
}

void SocketHandler::attach(Socket *socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->add(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket *socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->del(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket *socket)
{
    socket->close();
    m_sockpool.put(socket);
}