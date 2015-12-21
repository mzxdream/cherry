#include <net/m_epoll.h>
#include <sys/epoll.h>

MEpoll::MEpoll()
    :fd_(-1)
{
}

MEpoll::~MEpoll()
{
    Close();
}

MEpollError MEpoll::Attach(int fd)
{
    if (fd >= 0 && fd_ != fd)
    {
        MEpollError ret = MEpollError::No;
        if ((ret = Close()) != MEpollError::No)
        {
            return ret;
        }
        fd_ = fd;
    }
    return MEpollError::No;
}

int MEpoll::Detach()
{
    int old_fd = fd_;
    fd_ = -1;
    return old_fd;
}

MEpollError MEpoll::Create(size_t max_events)
{
    if (fd_ >= 0)
    {
        return MEpollError::Created;
    }
    if ((fd_ = epoll_create1(0)) == -1)
    {
        return CheckError();
    }
    event_list_.resize(max_events);
    return MEpollError::No;
}

MEpollError MEpoll::Close()
{
    if (fd_ < 0)
    {
        if (close(fd_) == -1)
        {
            return CheckError();
        }
        fd_ = -1;
    }
    return MEpollError::No;
}

MEpollError MEpoll::AddEvent(int sock, int event, MNetEvent &ev)
{
    int op = EPOLL_CTL_ADD;
    int prev_event = ev.GetEvent();
    if (prev_event != 0)
    {
        op = EPOLL_CTL_MOD;
    }
    struct epoll_event ee;
    ee.events = event | prev_event;
    ee.data.ptr = &ev;
    if (epoll_ctl(fd_, op, sock, &ee) == -1)
    {
        return CheckError();
    }
    ev.SetEvent(ee.events);
    return MEpollError::No;
}

MEpollError MEpoll::DelEvent(int sock, int event, MNetEvent &ev)
{
    struct epoll_event ee;
    ee.events = ev.GetEvent() & ~event;
    ee.data.ptr = &ev;
    int op = EPOLL_CTL_MOD;
    if (ee.events == 0)
    {
        op = EPOLL_CTL_DEL;
        ee.data.ptr = nullptr;
    }
    if (epoll_ctl(fd_, op, sock, &ee) == -1)
    {
        return CheckError();
    }
    ev.SetEvent(ee.events);
    return MEpollError::No;
}

MEpollError MEpoll::CloseEvent(int sock, MNetEvent &ev)
{
    struct epoll_event ee;
    ee.events = 0;
    ee.data.ptr = nullptr;
    if (epoll_ctl(fd_, EPOLL_CTL_DEL, sock, &ee) == -1)
    {
        return CheckError();
    }
    ev.SetEvent(ee.events);
    return MEpollError::No;
}

MEpollError MEpoll::ProcessEvents(int timeout)
{
    int ret = epoll_wait(fd_, &event_list_[0], event_list_.size(), timeout);
    if (ret == -1)
    {
        return CheckError();
    }
    if (ret == 0)
    {
        if (timeout == -1)
        {
            return MEpollError::Unknown;
        }
        return MEpollError::No;
    }
    for (int i = 0; i < ret; ++i)
    {
        MNetEvent *p_event = static_cast<MNetEvent*>(event_list_[i].data.ptr);
        if (!p_event)
        {
            continue;
        }
        int events = event_list_[i].events;
        if (events & (EPOLLERR|EPOLLHUP))
        {
            p_event->OnClose();
        }
        else if (events & EPOLLIN)
        {
            if (events & EPOLLRDHUP)
            {
                p_event->OnClose();
            }
            else
            {
                p_event->OnRead();
            }
        }
        else if (events & EPOLLOUT)
        {
            p_event->OnWrite();
        }
    }
    return MEpollError::No;
}

MEpollError MEpoll::CheckError()
{
    return MEpollError::Unknown;
}
