#include "WorkerThread.h"
#include "Singleton.h"
#include "Task.h"
#include "ThreadPool.h"
using namespace yazi::thread;

void WorkerThread::run() {
    sigset_t mask;
    if (0 != sigfillset(&mask)) {
        error("workerthread sigfillset fail!");
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL)) {
        error("workerthread pthread_sigmask fail!");
    }
    pthread_cleanup_push(cleanup, this);

    while (true) {
        // start wait for task
        m_mutex.lock();
        while (m_task == NULL)
            m_cond.wait(&m_mutex);
        m_mutex.unlock();

        int rc = 0;
        int old_state = 0;
        rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);

        m_task->run();
        m_task->destroy();
        m_task = NULL;

        Singleton<ThreadPool>::instance()->move_to_idle_list(this);

        rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
        pthread_testcancel(); // cancel-point
    }
    pthread_cleanup_pop(1);
}