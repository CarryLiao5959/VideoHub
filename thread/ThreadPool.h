#pragma once

#include <set>
#include "Thread.h"
#include "Mutex.h"
#include "AutoLock.h"
#include "Condition.h"
#include "Task.h"

namespace yazi
{
    namespace thread
    {
        class ThreadPool
        {
        public:
            ThreadPool();
            ~ThreadPool();

            int get_idle_thread_numbers();
            void move_to_idle_list(Thread *thread);
            void assign(Task *task);
            void create(int threads);

        private:
            int m_threads;

            std::set<Thread *> m_list_idle;
            std::set<Thread *> m_list_busy;

            Mutex m_mutex_idle;
            Mutex m_mutex_busy;

            Condition m_cond_idle;
            Condition m_cond_busy;
        };
    }
}
