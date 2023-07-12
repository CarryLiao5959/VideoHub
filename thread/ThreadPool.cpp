#include "ThreadPool.h"
#include "WorkerThread.h"

using namespace yazi::thread;

ThreadPool::ThreadPool() : m_threads(0)
{
}

ThreadPool::~ThreadPool()
{
}

int ThreadPool::get_idle_thread_numbers()
{
    return 0;
}

void ThreadPool::move_to_idle_list(Thread *thread)
{
}

void ThreadPool::assign(Task *task)
{
}

void ThreadPool::create(int threads)
{
    AutoLock lock(&m_mutex_idle);
    m_threads = threads;
    for (int i = 0; i < threads; i++)
    {
        Thread *thread = new WorkerThread();
        m_list_idle.insert(thread);
        thread->start();
    }
}