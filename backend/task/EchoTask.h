#pragma once

#include "Task.h"
using namespace yazi::thread;
#include "Logger.h"
using namespace yazi::util;

namespace yazi {
namespace task {

class EchoTask : public Task {
  public:
    EchoTask(void* data) { m_data = data; }
    virtual ~EchoTask(){}

    virtual void run(){
        debug("[EchoTask] echo data: %s", *(const char *)m_data);
    }
    virtual void destroy(){}
};

} // namespace task
} // namespace yazi