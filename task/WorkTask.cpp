#include "WorkTask.h"
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;
using namespace yazi::task;
using namespace yazi::socket;
#include <sstream>
using std::ostringstream;
#include <fstream>

void WorkTask::run() {
    debug("WorkTask running");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    memset(&msg_head, 0, sizeof(msg_head));
    int len = socket->recv((char *)(&msg_head), sizeof(msg_head));
    debug("recv len: %d", len);
    debug("recv MsgHead.flag: %s", msg_head.flag);
    debug("recv MsgHead.cmd: %d", msg_head.cmd);
    debug("recv MsgHead.len: %d", msg_head.len);

    if (len == 0) {
        error("socket closed by peer");
        handler->remove(socket);
        return;
    }
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
    if (len != sizeof(msg_head)) {
        error("recv msg head error length: %d, errno: %d", len, errno);
        handler->remove(socket);
        return;
    }

    switch (msg_head.cmd) {
    case 1:
        echo((int)(msg_head.len));
        break;
    case 2:
        text();
        break;
    case 3:
        img();
        break;
    default:
        error("Unknown command");
        break;
    }

    handler->attach(socket);
}

void WorkTask::echo(int msg_head_len) {
    debug("echo");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    char buf[recv_buff_size];
    // memset(buf, 0, recv_buff_size);
    // memcpy(buf, "OK", 2);
    // socket->send(buf, 2);
    // usleep(1);
    memset(buf, 0, recv_buff_size);
    int len = socket->recv(buf, msg_head_len);

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
    if (len != msg_head_len) {
        error("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
        handler->remove(socket);
        return;
    }

    info("recv len: %d, msg data: %s", len, buf);

    memset(buf, 0, recv_buff_size);
    memcpy(buf, "hello client", 13);
    socket->send(buf, 13);
}

void WorkTask::text() {
    debug("text");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/txt/file.txt";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s",filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[recv_buff_size];
    memset(buf, 0, recv_buff_size);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, recv_buff_size);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        // usleep(100);
        debug("send package %d", cnt++);
        memset(buf, 0, recv_buff_size);
    }
    memset(buf, 0, recv_buff_size);
    socket->send(buf, 0);
    debug("send package %d", cnt++);
    debug("text sent success");

    file.close();
}

void WorkTask::img() {
    debug("img");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/img/goal.png";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s", filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[recv_buff_size];
    memset(buf, 0, recv_buff_size);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, recv_buff_size);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        debug("send package %d", cnt++);
        memset(buf, 0, recv_buff_size);
    }
    memset(buf, 0, recv_buff_size);
    socket->send(buf, 0);
    debug("send package %d", cnt++);
    debug("img sent success");

    file.close();
}