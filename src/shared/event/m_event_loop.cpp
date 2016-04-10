#include <event/m_event_loop.h>
#include <fcntl.h>
#include <util/m_logger.h>
#include <util/m_timer.h>

MEventLoop::MEventLoop()
    :epoll_fd_(-1)
    ,cur_time_(0)
    ,interrupter_{-1, -1}
    ,io_event_count_(0)
{
}

MEventLoop::~MEventLoop()
{
    Clear();
}

MError MEventLoop::AddInterrupt()
{
    if (pipe(interrupter_) != 0)
    {
        MLOG(MGetLibLogger(), MERR, "create pipe failed, errno:", errno);
        return MError::Unknown;
    }
    fcntl(interrupter_[0], F_SETFL, O_NONBLOCK);
    fcntl(interrupter_[1], F_SETFL, O_NONBLOCK);
    epoll_event ee;
    ee.events = EPOLLIN | EPOLLERR | EPOLLET;
    ee.data.ptr = interrupter_;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, interrupter_[0], &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll ctl failed, errno:", errno);
        return MError::Unknown;
    }
    int tmp = 1;
    write(interrupter_[1], &tmp, sizeof(tmp));
    return MError::No;
}

MError MEventLoop::Init()
{
    if ((epoll_fd_ = epoll_create1(EPOLL_CLOEXEC)) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "create epoll failed, errno:", errno);
        return MError::Unknown;
    }
    MError err = AddInterrupt();
    if (err != MError::No)
    {
        return err;
    }
    cur_time_ = MTimer::GetTime();
    io_event_list_.resize(1024);
    io_event_count_ = 0;
    return MError::No;
}

void MEventLoop::Clear()
{
    if (epoll_fd_ >= 0)
    {
        close(epoll_fd_);
        epoll_fd_ = -1;
    }
    if (interrupter_[1] != interrupter_[0]
        && interrupter_[1] >= 0)
    {
        close(interrupter_[1]);
        interrupter_[1] = -1;
    }
    if (interrupter_[0] >= 0)
    {
        close(interrupter_[0]);
        interrupter_[0] = -1;
    }
}

int64_t MEventLoop::GetTime() const
{
    return cur_time_;
}

unsigned MEventLoop::GetIOEventCount() const
{
    return io_event_count_;
}

unsigned MEventLoop::GetAllEventCount() const
{
    return GetIOEventCount() + timer_events_.size()
        + circle_before_events_.size() + circle_after_events_.size();
}

MError MEventLoop::AddIOEvent(MIOEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    epoll_event ee;
    ee.events = p_event->GetEvents();
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, p_event->GetFD(), &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll add failed errno:", errno);
        return MError::Unknown;
    }
    ++io_event_count_;
    return MError::No;
}

MError MEventLoop::ModIOEvent(MIOEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    epoll_event ee;
    ee.events = p_event->GetEvents();
    ee.data.ptr = p_event;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, p_event->GetFD(), &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll mod failed errno:", errno);
        return MError::Unknown;
    }
    return MError::No;
}

MError MEventLoop::DelIOEvent(MIOEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    epoll_event ee;
    ee.events = 0;
    ee.data.ptr = nullptr;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, p_event->GetFD(), &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll del failed errno:", errno);
        return MError::Unknown;
    }
    --io_event_count_;
    return MError::No;
}

MError MEventLoop::AddTimerEvent(MTimerEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    int64_t timeout = GetTime() + p_event->GetTimeout();
    auto ret = timer_events_.insert(std::make_pair(timeout, p_event));
    if (!ret.second)
    {
        MLOG(MGetLibLogger(), MERR, "insert failed");
        return MError::Unknown;
    }
    p_event->SetLoopLocation(ret.first);
    return MError::No;
}

MError MEventLoop::DelTimerEvent(MTimerEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    if (p_event->GetLoopLocation() != timer_events_.end())
    {
        timer_events_.erase(p_event->GetLoopLocation());
    }
    return MError::No;
}

MError MEventLoop::AddCircleEventBeforeLoop(MCircleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    circle_before_events_.push_back(p_event);
    auto it = circle_before_events_.end();
    p_event->SetLoopLocation(--it);
    return MError::No;
}

MError MEventLoop::DelCircleEventBeforeLoop(MCircleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    if (p_event->GetLoopLocation() != circle_before_events_.end())
    {
        circle_before_events_.erase(p_event->GetLoopLocation());
    }
    return MError::No;
}

MError MEventLoop::AddCircleEventAfterLoop(MCircleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    circle_after_events_.push_back(p_event);
    auto it = circle_after_events_.end();
    p_event->SetLoopLocation(--it);
    return MError::No;
}

MError MEventLoop::DelCircleEventAfterLoop(MCircleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    if (p_event->GetLoopLocation() != circle_after_events_.end())
    {
        circle_after_events_.erase(p_event->GetLoopLocation());
    }
    return MError::No;
}

MError MEventLoop::Interrupt()
{
    epoll_event ee;
    ee.events = EPOLLIN | EPOLLERR | EPOLLET;
    ee.data.ptr = interrupter_;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, interrupter_[0], &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll ctl failed errno:", errno);
        return MError::Unknown;
    }
    return MError::No;
}

MError MEventLoop::DispatchEvents(int timeout)
{
    return MError::No;
}

int64_t MEventLoop::GetLeastTime()
{
    return -1;
}

MError MEventLoop::DispatchIOEvents(int timeout)
{
    return MError::No;
}

MError MEventLoop::DispatchTimerEvents()
{
    return MError::No;
}

MError MEventLoop::DispatchCircleEventsBeforeLoop()
{
    return MError::No;
}

MError MEventLoop::DispatchCircleEventsAfterLoop()
{
    return MError::No;
}

MError MNetEventLoop::ProcessEvents()
{
    int max_events = epoll_wait(epoll_fd_, &event_list_[0], event_list_.size(), -1);
    if (max_events == -1)
    {
        if (errno == EINTR)
        {
            return MError::No;
        }
        MLOG(MGetLibLogger(), MERR, "epoll wait failed errno:", errno);
        return MError::Unknown;
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
            p_event->OnErrorCallback(MError::Disconnect);
            continue;
        }
        if (events & EPOLLIN)
        {
            if (events & EPOLLRDHUP)
            {
                p_event->OnErrorCallback(MError::Disconnect);
                continue;
            }
            p_event->OnReadCallback();
        }
        if (events & EPOLLOUT)
        {
            p_event->OnWriteCallback();
        }
    }
    return MError::No;
}