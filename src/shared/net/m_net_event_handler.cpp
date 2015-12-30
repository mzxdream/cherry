#include <net/m_net_event_handler.h>
#include <unistd.h>
#include <net/m_net_event.h>

MNetEventHandler::MNetEventHandler()
    :epoll_fd_(-1)
    ,interrupter_({-1, -1})
    ,event_count_(0)
{
}

MNetEventHandler::~MNetEventHandler()
{
    Close();
}

size_t MNetEventHandler::GetEventCount() const
{
    return event_count_;
}

MNetError MNetEventHandler::Create()
{
    if (epoll_fd_ >= 0)
    {
        return MNetError::Created;
    }
    if ((epoll_fd_ = epoll_create1(EPOLL_CLOEXEC)) == -1)
    {
        return CheckError();
    }
    if (interrupter_[0] == -1)
    {
        if (pipe(interrupter_) != 0)
        {
            return CheckError();
        }
        fcntl(interrupter_[0], F_SETFL, O_NONBLOCK);
        fcntl(interrupter_[1], F_SETFL, O_NONBLOCK);
        epoll_event ee;
        ee.events = EPOLLIN | EPOLLERR | EPOLLET;
        ee.data.ptr = interrupter_;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, interrupter_[0], &ee) == -1)
        {
            return CheckError();
        }
        int tmp = 1;
        write(interrupter_[1], &tmp, sizeof(tmp));
    }
    return MNetError::No;
}

MNetError MNetEventHandler::Close()
{
    if (epoll_fd_ >= 0)
    {
        if (close(epoll_fd_) == -1)
        {
            return CheckError();
        }
        epoll_fd_ = -1;
    }
    if (interrupter_[0] >= 0)
    {
        if (interrupter_[0] != interrupter_[1])
        {
            if (close(interrupter_[1]) == -1)
            {
                return CheckError();
            }
            interrupter_[1] = -1;
        }
        if (close(interrupter_[0]) == -1)
        {
            return CheckError();
        }
        interrupter_[0] = -1;
    }
    return MNetError::No;
}

MNetError MNetEventHandler::AddEvent(int fd, int events, MNetEvent *p_event)
{
    epoll_event ee;
    ee.events = events;
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ee) == -1)
    {
        return CheckError();
    }
    ++event_count_;
    return MNetError::No;
}

MNetError MNetEventHandler::ModEvent(int fd, int events, MNetEvent *p_event)
{
    epoll_event ee;
    ee.events = events;
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ee) == -1)
    {
        return CheckError();
    }
    return MNetEvent::No;
}

MNetError MNetEventHandler::DelEvent(int fd)
{
    epoll_event ee;
    ee.events = 0;
    ee.data.ptr = nullptr;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ee) == -1)
    {
        return CheckError();
    }
    --event_count_;
    return MNetError::No;
}

MNetError MNetEventHandler::ProcessEvents()
{
    int max_events = epoll_wait(epoll_fd_, event_list_, M_NET_EVENT_MAX_EVENTS, -1);
    if (max_events == -1)
    {
        return CheckError();
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
        }
        else if (events & EPOLLIN)
        {
            if (events & EPOLLRDHUP)
            {
                p_event->OnErrorCallback(MNetError::Disconnect);
            }
            else
            {
                p_event->OnReadCallback();
            }
        }
        else if (events & EPOLLOUT)
        {
            p_event->OnWriteCallback();
        }
    }
    return MNetError::No;
}

MNetError MNetEventHandler::Interrupt()
{
    epoll_event ee;
    ee.events = EPOLLIN | EPOLLERR | EPOLLET;
    ee.data.ptr = &interrupter_;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, interrupter_[0], &ee) == -1)
    {
        return CheckError();
    }
    return MNetError::No;
}

void MNetEventHandler::DoRun()
{
    ProcessEvents();
}

MNetError MNetEventHandler::CheckError()
{
    return MNetError::Unknown;
}
