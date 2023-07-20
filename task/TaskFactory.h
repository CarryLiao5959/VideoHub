#pragma once

#include "Socket.h"
using namespace yazi::socket;
#include "WorkTask.h"
using namespace yazi::task;
#include "Task.h"
using namespace yazi::thread;

namespace yazi {
namespace task {
class TaskFactory {
  public:
    static Task *create(Socket *socket) { return new WorkTask(socket); }
};
} // namespace task
} // namespace yazi