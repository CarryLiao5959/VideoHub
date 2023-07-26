#include "WorkTask.h"

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;

#include "SocketHandler.h"
using namespace yazi::socket;

using namespace yazi::task;

#include <sstream>
using std::ostringstream;

WorkTask::WorkTask(Socket *socket) : Task(socket) {
    debug("new WorkTask sockfd: %d",socket->getfd());
}

WorkTask::~WorkTask() {}

void WorkTask::run() {
    debug("WorkTask running");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *> (m_data);
    char buf[recv_buff_size];
    memset(buf, 0, recv_buff_size);
    int len = socket->recv(buf, sizeof(buf));
    if (len == -1) {
        switch (errno) {
        case EWOULDBLOCK:
                error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
                handler->attach(socket);
                return;
        case EINTR:
                error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
                handler->attach(socket);
                return;
        }
    }
    info("recv len: %d, msg data: %s", len, buf);
    memset(buf, 0, recv_buff_size);
    memcpy(buf,"hello client",13);
    socket->send(buf, 13);
    handler->attach(socket);
}

void WorkTask::destroy() {
    debug("work job destory");
    delete this;
}