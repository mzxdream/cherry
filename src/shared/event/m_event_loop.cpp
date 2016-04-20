#include <event/m_event_loop.h>
#include <fcntl.h>
#include <util/m_logger.h>
#include <util/m_timer.h>
#include <unistd.h>

MEventLoop::MEventLoop()
    :epoll_fd_(-1)
    ,cur_time_(0)
    ,interrupter_{-1, -1}
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
    ee.data.fd = interrupter_[0];
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
    UpdateTime();
    io_event_list_.resize(1024);
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
    cur_time_ = MTimer::GetTime();
}

size_t MEventLoop::GetIOEventCount() const
{
    return io_events_.size();
}

size_t MEventLoop::GetTimerEventCount() const
{
    return timer_events_.size();
}

size_t MEventLoop::GetBeforeEventCount() const
{
    return before_events_.size();
}

size_t MEventLoop::GetAfterEventCount() const
{
    return after_events_.size();
}

size_t MEventLoop::GetAllEventCount() const
{
    return GetIOEventCount() + GetTimerEventCount()
        + GetBeforeEventCount() + GetAfterEventCount();
}

MError MEventLoop::AddIOEvent(int fd, unsigned events, const std::function<void (unsigned)> &cb)
{
    if (fd < 0)
    {
        MLOG(MGetLibLogger(), MERR, "fd is less than 0");
        return MError::Invalid;
    }
    if (!cb)
    {
        MLOG(MGetLibLogger(), MERR, "callback is invalid");
        return MError::Invalid;
    }
    epoll_event ee;
    ee.events = events;
    ee.data.fd = fd;
    int op = EPOLL_CTL_ADD;
    auto iter = io_events_.find(fd);
    if (iter == io_events_.end())
    {
        if ((events & (MIOEVENT_IN | MIOEVENT_OUT | MIOEVENT_RDHUP)) == 0)
        {
            MLOG(MGetLibLogger(), MERR, "events is not in out or rdhup");
            return MError::Invalid;
        }
    }
    else
    {
        op = EPOLL_CTL_MOD;
        ee.events |= iter->second.first;
    }
    if (epoll_ctl(epoll_fd_, op, fd, &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll add failed errno:", errno);
        return MError::Unknown;
    }
    if (iter == io_events_.end())
    {
        io_events_.insert(std::make_pair(fd, std::make_pair(static_cast<unsigned>(ee.events), cb)));
    }
    else
    {
        iter->second.first = ee.events;
        iter->second.second = cb;
    }
    return MError::No;
}

MError MEventLoop::DelIOEvent(int fd, unsigned events)
{
    auto iter = io_events_.find(fd);
    if (iter == io_events_.end())
    {
        return MError::No;
    }
    iter->second.first &= ~events;
    epoll_event ee;
    ee.data.fd = fd;
    ee.events = 0;
    int op = EPOLL_CTL_DEL;
    if (iter->second.first & (MIOEVENT_IN | MIOEVENT_OUT | MIOEVENT_RDHUP))
    {
        ee.events = iter->second.first;
        op = EPOLL_CTL_MOD;
    }
    if (epoll_ctl(epoll_fd_, op, fd, &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll del failed errno:", errno);
        return MError::Unknown;
    }
    if (op == EPOLL_CTL_DEL)
    {
        io_events_.erase(iter);
    }
    return MError::No;
}

std::pair<MTimerEventLocation, MError> MEventLoop::AddTimerEvent(int64_t start_time, const std::function<void ()> &cb)
{
    if (!cb)
    {
        MLOG(MGetLibLogger(), MERR, "callback is invalid");
        return std::make_pair(timer_events_.end(), MError::Invalid);
    }
    auto ret = timer_events_.insert(std::make_pair(start_time, cb));
    return std::make_pair(ret, MError::No);
}

MError MEventLoop::DelTimerEvent(const MTimerEventLocation &location)
{
    timer_events_.erase(location);
    return MError::No;
}

std::pair<MBeforeEventLocation, MError> MEventLoop::AddBeforeEvent(const std::function<void ()> &cb)
{
    if (!cb)
    {
        MLOG(MGetLibLogger(), MERR, "callback is invalid");
        return std::make_pair(before_events_.end(), MError::Invalid);
    }
    before_events_.push_back(cb);
    auto it = before_events_.end();
    return std::make_pair(--it, MError::No);
}

MError MEventLoop::DelBeforeEvent(const MBeforeEventLocation &location)
{
    before_events_.erase(location);
    return MError::No;
}

std::pair<MAfterEventLocation, MError> MEventLoop::AddAfterEvent(const std::function<void ()> &cb)
{
    if (!cb)
    {
        MLOG(MGetLibLogger(), MERR, "callback is invalid");
        return std::make_pair(after_events_.end(), MError::Invalid);
    }
    after_events_.push_back(cb);
    auto it = after_events_.end();
    return std::make_pair(--it, MError::No);
}

MError MEventLoop::DelAfterEvent(const MAfterEventLocation &location)
{
    after_events_.erase(location);
    return MError::No;
}

MError MEventLoop::Interrupt()
{
    epoll_event ee;
    ee.events = EPOLLIN | EPOLLERR | EPOLLET;
    ee.data.fd = interrupter_[0];
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, interrupter_[0], &ee) == -1)
    {
        MLOG(MGetLibLogger(), MERR, "epoll ctl failed errno:", errno);
        return MError::Unknown;
    }
    return MError::No;
}

MError MEventLoop::DispatchEvents(int timeout)
{
    MError err = DispatchBeforeEvents();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchBeforeEvents failed");
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
            err = DispatchIOEvents(false, std::min(timer_events_.begin()->first, cur_time_ + timeout));
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
    err = DispatchAfterEvents();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "DispatchAfterEvents failed");
        return err;
    }
    return MError::No;
}

MError MEventLoop::DispatchIOEvents(bool forever, int64_t outdate_time)
{
    int timeout = -1;
    bool interrupt = false;
    int count = 48;
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
                int fd = io_event_list_[i].data.fd;
                if (fd == interrupter_[0])
                {
                    interrupt = true;
                    continue;
                }
                auto iter = io_events_.find(fd);
                if (iter == io_events_.end())
                {
                    MLOG(MGetLibLogger(), MWARN, "fd:", fd, " is not found");
                    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &io_event_list_[i]);
                    continue;
                }
                (iter->second.second)(io_event_list_[i].events);
            }
        }
        if (interrupt)
        {
            break;
        }
        if (nevents > 0)
        {
            if (static_cast<size_t>(nevents) == io_event_list_.size() && (--count > 0))
            {
                outdate_time = cur_time_;
                continue;
            }
            break;
        }
        UpdateTime();
        if (!forever && (outdate_time <= cur_time_))
        {
            break;
        }
    } while (true);
    return MError::No;
}

MError MEventLoop::DispatchTimerEvents()
{
    auto it = timer_events_.begin();
    while (it != timer_events_.end())
    {
        if (it->first > cur_time_)
        {
            break;
        }
        (it->second)();
        timer_events_.erase(it);
        it = timer_events_.begin();
    }
    return MError::No;
}

MError MEventLoop::DispatchBeforeEvents()
{
    std::list<std::function<void ()> > event_list;
    event_list.swap(before_events_);
    auto it = event_list.begin();
    while (it != event_list.end())
    {
        (*it)();
        it = event_list.erase(it);
    }
    return MError::No;
}

MError MEventLoop::DispatchAfterEvents()
{
    std::list<std::function<void ()> > event_list;
    event_list.swap(after_events_);
    auto it = event_list.begin();
    while (it != event_list.end())
    {
        (*it)();
        it = event_list.erase(it);
    }
    return MError::No;
}
