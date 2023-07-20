#include "WorkTask.h"

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;

#include "SocketHandler.h"
using namespace yazi::socket;

using namespace yazi::task;

#include <sstream>
using std::ostringstream;

WorkTask::WorkTask(Socket *socket) : Task(socket) {}

WorkTask::~WorkTask() {}

void WorkTask::run() {}

void WorkTask::destroy() {
    debug("work job destory");
    delete this;
}
