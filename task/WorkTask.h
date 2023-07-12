#pragma once

#include <sstream>
using std::ostringstream;

#include "Task.h"
using namespace yazi::thread;
#include "Socket.h"
#include "SocketHandler.h"
using namespace yazi::socket;
#include "Singleton.h"
using namespace yazi::util;

namespace yazi
{
    namespace task
    {
        class WorkTask : public Task
        {
        public:
            WorkTask(Socket *socket);
            virtual ~WorkTask();
        };
    }
}
