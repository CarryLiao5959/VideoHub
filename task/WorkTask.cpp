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
    debug("new WorkTask socket %d",socket->getfd());
}

WorkTask::~WorkTask() {}

void WorkTask::run() {
    debug("[WorkTask] data: %s", *(Socket *)m_data);
}

void WorkTask::destroy() {
    debug("work job destory");
    delete this;
}