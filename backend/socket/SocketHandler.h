#pragma once

#include "Socket.h"
#include "EventPoller.h"
#include "ObjectPool.h"
#include "AutoLock.h"
#include "Mutex.h"
#include "Singleton.h"
using namespace yazi::util;
#include "ServerSocket.h"
#include "Task.h"
#include <string>
#include <vector>
using namespace std;

namespace yazi {
namespace socket {
class SocketHandler {
  public:
    SocketHandler();
    ~SocketHandler();

    void listen(const string &ip, int port);
    void handle(int max_connects, int wait_time);

    void broadcast(const std::string& message);

    void attach(Socket *socket);
    void detach(Socket *socket);
    void remove(Socket *socket);

  private:
    EventPoller *m_epoll;
    Socket *m_server;
    ObjectPool<Socket> m_sockpool;
    vector<Socket*> active_sock;
    Mutex m_mutex;
};
} // namespace socket
} // namespace yazi