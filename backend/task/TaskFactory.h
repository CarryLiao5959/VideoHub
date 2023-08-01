#pragma once

#include "Socket.h"
#include "Task.h"
#include "HLSTask.h"
#include "WorkTask.h"
using namespace yazi::socket;
using namespace yazi::thread;

namespace yazi {
namespace task {
class TaskFactory {
  public:
    static Task *create(Socket *socket) {
        return new WorkTask(socket);
        // return new HLSTask(socket);
    }
};
} // namespace task
} // namespace yazi