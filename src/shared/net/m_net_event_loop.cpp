#include <net/m_net_event_loop.h>
#include <net/m_net_event.h>

MNetEventLoop::MNetEventLoop(size_t single_process_events)
    :epoll_fd_(-1)
    ,event_list_(single_process_events)
    ,interrupter_({-1, -1})
    ,event_count_(0)
{
}

MNetEventLoop::~MNetEventLoop()
{
    Close();
}

size_t MNetEventLoop::GetEventCount() const
{
    return event_count_;
}

MNetError MNetEventLoop::Create()
{
    if (epoll_fd_ >= 0)
    {
        return MNetError::Created;
    }
    if ((epoll_fd_ = epoll_create1(EPOLL_CLOEXEC)) == -1)
    {
        return MNetCheckLastError();
    }
    if (interrupter_[0] == -1)
    {
        if (pipe(interrupter_) != 0)
        {
            return MNetCheckLastError();
        }
        fcntl(interrupter_[0], F_SETFL, O_NONBLOCK);
        fcntl(interrupter_[1], F_SETFL, O_NONBLOCK);
        epoll_event ee;
        ee.events = EPOLLIN | EPOLLERR | EPOLLET;
        ee.data.ptr = interrupter_;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, interrupter_[0], &ee) == -1)
        {
            return MNetCheckLastError();
        }
        int tmp = 1;
        write(interrupter_[1], &tmp, sizeof(tmp));
    }
    return MNetError::No;
}

MNetError MNetEventLoop::Close()
{
    if (epoll_fd_ >= 0)
    {
        if (close(epoll_fd_) == -1)
        {
            return MNetCheckLastError();
        }
        epoll_fd_ = -1;
    }

    if (interrupter_[1] != interrupter_[0] && interrupter_[1] >= 0)
    {
        if (close(interrupter_[1]) == -1)
        {
            return MNetCheckLastError();
        }
        interrupter_[1] = -1;
    }

    if (interrupter_[0] >= 0)
    {
        if (close(interrupter_[0]) == -1)
        {
            return MNetCheckLastError();
        }
        interrupter_[0] = -1;
    }
    return MNetError::No;
}

MNetError MNetEventLoop::AddEvent(int fd, int events, MNetEvent *p_event)
{
    epoll_event ee;
    ee.events = events;
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ee) == -1)
    {
        return MNetCheckLastError();
    }
    ++event_count_;
    return MNetError::No;
}

MNetError MNetEventLoop::ModEvent(int fd, int events, MNetEvent *p_event)
{
    epoll_event ee;
    ee.events = events;
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ee) == -1)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

MNetError MNetEventLoop::DelEvent(int fd)
{
    epoll_event ee;
    ee.events = 0;
    ee.data.ptr = nullptr;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ee) == -1)
    {
        return MNetCheckLastError();
    }
    --event_count_;
    return MNetError::No;
}

MNetError MNetEventLoop::ProcessEvents()
{
    int max_events = epoll_wait(epoll_fd_, &event_list_[0], event_list_.size(), -1);
    if (max_events == -1)
    {
        return MNetCheckLastError();
    }
    for (int i = 0; i < max_events; ++i)
    {
        if (event_list_[i].data.ptr == interrupter_)
        {
            continue;
        }
        MNetEvent *p_event = static_cast<MNetEvent*>(event_list_[i].data.ptr);
        if (!p_event)
        {
            continue;
        }
        int events = event_list_[i].events;
        if (events & (EPOLLERR|EPOLLHUP))
        {
            p_event->OnErrorCallback(MNetError::Disconnect);
            continue;
        }
        if (events & EPOLLIN)
        {
            if (events & EPOLLRDHUP)
            {
                p_event->OnErrorCallback(MNetError::Disconnect);
                continue;
            }
            p_event->OnReadCallback();
        }
        if (events & EPOLLOUT)
        {
            p_event->OnWriteCallback();
        }
    }
    return MNetError::No;
}

MNetError MNetEventLoop::Interrupt()
{
    epoll_event ee;
    ee.events = EPOLLIN | EPOLLERR | EPOLLET;
    ee.data.ptr = &interrupter_;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, interrupter_[0], &ee) == -1)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}
