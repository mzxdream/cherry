#include <event/m_timer_event.h>
#include <event/m_event_loop.h>
#include <util/m_logger.h>

MTimerEvent::MTimerEvent()
    :event_active_(false)
    ,p_event_loop_(nullptr)
    ,start_time_(0)
    ,timeout_(0)
    ,repeat_count_(0)
    ,cb_(nullptr)
{
}

MTimerEvent::~MTimerEvent()
{
    DisableEvents();
}

void MTimerEvent::SetLoopLocation(std::multimap<int64_t, MTimerEvent*>::iterator location)
{
    loop_location_ = location;
}

std::multimap<int64_t, MTimerEvent*>::iterator MTimerEvent::GetLoopLocation() const
{
    return loop_location_;
}

void MTimerEvent::SetEventActive(bool active)
{
    event_active_ = active;
}

bool MTimerEvent::IsEventActive() const
{
    return event_active_;
}

int64_t MTimerEvent::GetStartTime() const
{
    return start_time_;
}

int MTimerEvent::GetTimeout() const
{
    return timeout_;
}

bool MTimerEvent::NeedRepeat() const
{
    return repeat_count_ != 0;
}

MError MTimerEvent::EnableEvents(MEventLoop *p_event_loop, const std::function<void ()> &cb
        , int timeout, int repeat_count, int64_t start_time)
{
    if (event_active_)
    {
        MError err = DisableEvents();
        if (err != MError::No)
        {
            return err;
        }
    }
    if (!p_event_loop)
    {
        MLOG(MGetLibLogger(), MERR, "event loop is invalid");
        return MError::Invalid;
    }
    if (!cb)
    {
        MLOG(MGetLibLogger(), MERR, "cb is invalid");
        return MError::Invalid;
    }
    if (repeat_count != 0 && timeout <= 0)
    {
        MLOG(MGetLibLogger(), MERR, "timout is less than 0");
        return MError::Unknown;
    }
    if (start_time < p_event_loop->GetTime())
    {
        MLOG(MGetLibLogger(), MERR, "start time is less than loop time");
        return MError::Unknown;
    }
    p_event_loop_ = p_event_loop;
    cb_ = cb;
    timeout_ = timeout;
    repeat_count_ = repeat_count;
    start_time_ = start_time;
    return p_event_loop_->AddTimerEvent(this);
}

MError MTimerEvent::EnableEvents(MEventLoop *p_event_loop, const std::function<void ()> &cb
        , int timeout, int repeat_count)
{
    return EnableEvents();
}

MError MTimerEvent::DisableEvents()
{
    if (!event_active_)
    {
        return MError::No;
    }
    return p_event_loop_->DelTimerEvent(this);
}

void MTimerEvent::OnCallback()
{
    cb_();
    if (repeat_count_ > 0)
    {
        --repeat_count_;
    }
}
