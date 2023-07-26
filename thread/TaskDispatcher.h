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
            TaskDispatcher();
            ~TaskDispatcher();

            void init(int threads);
            void assign(Task *task);
            
            virtual void run() override;

          protected:
            std::list<Task *> m_tasks;
        };
    }
}