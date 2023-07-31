#pragma once

#include "Socket.h"
#include "Task.h"
#include "SocketHandler.h"
using namespace yazi::socket;
using namespace yazi::thread;

namespace yazi {
namespace task {

const unsigned int hls_buf_size = 1024;

class HLSTask : public Task {

  public:
    HLSTask(Socket *socket);

    virtual ~HLSTask();

    virtual void run() override;

    virtual void destroy() override;

};

} // namespace task
} // namespace yazi
