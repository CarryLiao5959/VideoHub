#pragma once

#include <list>
#include <pthread.h>
#include <signal.h>

#include "Task.h"
#include "Thread.h"

namespace yazi
{
    namespace thread
    {
        class TaskDispatcher : public Thread
        {
        public:
            void init(int threads);
            void assign(Task *task);
            void handle(Task *task);
            virtual void run();

        protected:
            std::list<Task *> m_tasks;
        };
    }
}