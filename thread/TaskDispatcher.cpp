#include "TaskDispatcher.h"
#include "Singleton.h"
#include "Logger.h"
using namespace yazi::util;
#include "ThreadPool.h"
using namespace yazi::thread;
#include <unistd.h>

TaskDispatcher::TaskDispatcher() {}
TaskDispatcher::~TaskDispatcher() {}

void TaskDispatcher::init(int threads) {
    debug("init TaskDispatcher");
    Singleton<ThreadPool>::instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task *task) {
    m_mutex.lock();
    m_tasks.push_back(task);
    m_mutex.unlock();
    m_cond.signal();
}

void TaskDispatcher::run() {
    sigset_t mask;
    if (0 != sigfillset(&mask)) {
        return;
    }
    // disable signal interrupts
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL)) {
        return;
    }
    while (true) {
        m_mutex.lock();
        while (m_tasks.empty()){
            debug("TaskDispatcher m_tasks is empty");
            m_cond.wait(&m_mutex);
        }
        // got task
        Task *task = m_tasks.front();
        m_tasks.pop_front();
        m_mutex.unlock();

        ThreadPool *threadpool = Singleton<ThreadPool>::instance();
        if (threadpool->get_idle_thread_numbers() > 0) {
            debug("TaskDispatcher: let threadpool to assign task");
            threadpool->assign(task);
        } else {
            debug("there is no idle_thread in threadpool");
            m_mutex.lock();
            m_tasks.push_front(task);
            m_mutex.unlock();
        }
    }
}