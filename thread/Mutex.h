#pragma once
#include <pthread.h>

namespace yazi
{
    namespace thread
    {
        class Mutex
        {
            friend class Condition;

        public:
            Mutex();
            ~Mutex();

            int lock();
            int try_lock();
            int unlock();

        private:
            pthread_mutex_t m_mutex;
        };
    }
}
