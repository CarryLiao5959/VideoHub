#include "Task.h"
#include "AutoLock.h"

using namespace yazi::thread;

Task::Task() : m_data(nullptr)
{
}

Task::Task(void *data) : m_data(data)
{
}

Task::~Task()
{
}

void Task::run()
{
}

void Task::destroy()
{
}