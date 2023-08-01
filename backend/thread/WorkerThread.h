#pragma once

#include <pthread.h>
#include <signal.h>
#include "Thread.h"
#include "Logger.h"
using namespace yazi::util;
namespace yazi
{
    namespace thread
    {

        class WorkerThread : public Thread
        {
        public:
            WorkerThread():Thread(){}
            WorkerThread(int tid) : Thread(tid) {}
            virtual ~WorkerThread(){}

            virtual void run();

            static void cleanup(void *ptr){
                info("worker thread cleanup handler: %x", ptr);
            }
        };

    }
}