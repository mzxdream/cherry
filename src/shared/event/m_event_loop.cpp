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

void MEventLoop::UpdateTime()
{
    cur_time_ = CTimer::GetTime();
}

unsigned MEventLoop::GetIOEventCount() const
{
    return io_event_count_;
}

unsigned MEventLoop::GetAllEventCount() const
{
    return io_event_count_ + timer_events_.size()
        + before_idle_events_.size() + after_idle_events_.size();
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
    auto ret = timer_events_.insert(std::make_pair(p_event->GetStartTime(), p_event));
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
        p_event->SetLoopLocation(timer_events_.end());
    }
    return MError::No;
}

MError MEventLoop::AddBeforeIdleEvent(MIdleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    before_idle_events_.push_back(p_event);
    auto it = before_idle_events_.end();
    p_event->SetLoopLocation(--it);
    return MError::No;
}

MError MEventLoop::DelBeforeIdleEvent(MIdleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    if (p_event->GetLoopLocation() != before_idle_events_.end())
    {
        before_idle_events_.erase(p_event->GetLoopLocation());
        p_event->SetLoopLocation(before_idle_events_.end());
    }
    return MError::No;
}

MError MEventLoop::AddAfterIdleEvent(MIdleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    after_idle_events_.push_back(p_event);
    auto it = after_idle_events_.end();
    p_event->SetLoopLocation(--it);
    return MError::No;
}

MError MEventLoop::DelAfterIdleEvent(MIdleEvent *p_event)
{
    if (!p_event)
    {
        MLOG(MGetLibLogger(), MERR, "event is invalid");
        return MError::Invalid;
    }
    if (p_event->GetLoopLocation() != after_idle_events_.end())
    {
        after_idle_events_.erase(p_event->GetLoopLocation());
        p_event->SetLoopLocation(after_idle_events_.end());
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
    MError err = DispatchBeforeIdleEvents();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchBeforeIdleEvents failed");
        return err;
    }
    if (timeout < 0)
    {
        if (timer_events_.empty())
        {
            err = DispatchIOEvents(true, 0);
        }
        else
        {
            err = DispatchIOEvents(false, timer_events_.begin()->first);
        }
    }
    else
    {
        if (timer_events_.empty())
        {
            err = DispatchIOEvents(false, cur_time_ + timeout);
        }
        else
        {
            err = DispatchIOEvents(false, std::min(timer_events_.begin()->first, cur_time_ + timeout);
        }
    }
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchIOEvents failed");
        return err;
    }
    UpdateTime();
    err = DispatchTimerEvents();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchTimerEvents failed");
        return err;
    }
    err = DispatchAfterIdleEvents();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchAfterIdleEvents failed");
        return err;
    }
    return MError::No;
}

MError MEventLoop::DispatchIOEvents(bool forever, int64_t outdate_time)
{
    int count = 48;
    int timeout = -1;
    bool interrupt = false;
    do
    {
        if (!forever)
        {
            timeout = std::max(0, static_cast<int>(outdate_time - cur_time_));
        }
        int nevents = epoll_wait(epoll_fd_, &io_event_list_[0], io_event_list_.size(), timeout);
        if (nevents == -1)
        {
            if (errno != EINTR)
            {
                MLOG(MGetLibLogger(), MERR, "epoll wait failed errno:", errno);
                return MError::Unknown;
            }
        }
        else
        {
            for (int i = 0; i < nevents; ++i)
            {
                if (io_event_list_[i].data.ptr == interrupter_)
                {
                    interrupt = true;
                    continue;
                }
                MIOEvent *p_event = static_cast<MIOEvent*>(io_event_list_[i].data.ptr);
                if (!p_event)
                {
                    MLOG(MGetLibLogger(), MERR, "p_event is nullptr errno:", errno);
                    continue;
                }
                int events = io_event_list_[i].events;
                if (events & (EPOLLERR|EPOLLHUP))
                {
                    MLOG(MGetLibLogger(), MINFO, "socket error:", errno);
                    p_event->OnCallback(0, MError::Unknown);
                    continue;
                }
                int ev = 0;
                if (events & EPOLLIN)
                {
                    if (events & EPOLLRDHUP)
                    {
                        p_event->OnCallback(0, MError::Disconnect);
                        continue;
                    }
                    ev |= IOEVENT_IN;
                }
                if (events & EPOLLOUT)
                {
                    ev |= IOEVENT_OUT;
                }
                p_event->OnCallback(ev, MError::No);
            }
        }
        UpdateTime();
    } while ((forever || (outdate_time > cur_time_)) && (--count != 0) && !interrupt);
    return MError::No;
}

MError MEventLoop::DispatchTimerEvents()
{
    MTimerEvent *p_event = nullptr;
    for (auto it = timer_events_.begin(); it != timer_events_.end(); it = timer_events_.begin())
    {
        if (it->first > cur_time_)
        {
            break;
        }
        p_event = it->second;
        timer_events_.erase(it);
        p_event->OnCallback();
        if (!p_event->NeedRepeat())
        {
            p_event->SetLoopLocation(timer_events_.end());
        }
        else
        {
            int64_t next_time = cur_time_ + p_event->GetTimeout();
            auto ret = timer_events_.insert(std::make_pair(next_time, p_event));
            if (!ret.second)
            {
                MLOG(MGetLibLogger(), MERR, "insert failed");
                return MError::Unknown;
            }
            p_event->SetLoopLocation(ret.first);
        }
    }
    return MError::No;
}

MError MEventLoop::DispatchBeforeIdleEvents()
{
    auto it = before_idle_events_.begin();
    for (; it != before_idle_events_.end(); )
    {
        MIdleEvent *p_event = *it;
        if (!p_event)
        {
            it = before_idle_events_.erase(it);
            continue;
        }
        p_event->OnCallback();
        if (!p_event->NeedRepeat())
        {
            it = before_idle_events_.erase(it);
            p_event->SetLoopLocation(before_idle_events_.end());
            continue;
        }
        ++it;
    }
    return MError::No;
}

MError MEventLoop::DispatchAfterIdleEvents()
{
    auto it = after_idle_events_.begin();
    for (; it != after_idle_events_.end(); )
    {
        MIdleEvent *p_event = *it;
        if (!p_event)
        {
            it = after_idle_events_.erase(it);
            continue;
        }
        p_event->OnCallback();
        if (!p_event->NeedRepeat())
        {
            it = after_idle_events_.erase(it);
            p_event->SetLoopLocation(after_idle_events_.end());
            continue;
        }
        ++it;
    }
    return MError::No;
}
