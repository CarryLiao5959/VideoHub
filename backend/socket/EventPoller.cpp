#include "EventPoller.h"
#include "Logger.h"
using namespace yazi::socket;
using namespace yazi::util;

EventPoller::EventPoller(bool et) : m_epfd(0), m_max_connections(0), m_events(nullptr), m_et(et)
{
}

EventPoller::~EventPoller()
{
    if (m_epfd > 0)
    {
        close(m_epfd);
        m_epfd = 0;
    }

    if (m_events != nullptr)
    {
        delete[] m_events;
        m_events = nullptr;
    }
}

void EventPoller::ctrl(int fd, void *ptr, __uint32_t events, int op)
{
    struct epoll_event ev;
    ev.data.ptr = ptr;
    if (m_et)
    {
        ev.events = events | EPOLLET;
    }
    else
    {
        ev.events = events;
    }
    epoll_ctl(m_epfd, op, fd, &ev);
}

void EventPoller::create(int max_connections)
{
    m_max_connections = max_connections;
    m_epfd = epoll_create(max_connections + 1);
    if (m_epfd < 0)
    {
        return;
    }
    if (m_events != nullptr)
    {
        delete[] m_events;
        m_events = nullptr;
    }
    m_events = new epoll_event[max_connections + 1];
}

void EventPoller::add(int fd, void *ptr, __uint32_t events)
{
    ctrl(fd, ptr, events, EPOLL_CTL_ADD);
    debug("add event to epoll sockfd: %d", fd);
}

void EventPoller::del(int fd, void *ptr, __uint32_t events)
{
    ctrl(fd, ptr, events, EPOLL_CTL_DEL);
    debug("delete event from epoll sockfd: %d", fd);
}

int EventPoller::wait(int millsecond)
{
    return epoll_wait(m_epfd, m_events, m_max_connections + 1, millsecond);
}
