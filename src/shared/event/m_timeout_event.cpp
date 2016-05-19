#include <event/m_timeout_event.h>

MTimeoutEvent::MTimeoutEvent()
    :p_event_loop_(nullptr)
    ,timeout_(0)
    ,repeated_(0)
{
}

MTimeoutEvent::~MTimeoutEvent()
{
    Clear();
}

MError MTimeoutEvent::Init(MEventLoop *p_event_loop, const std::function<void ()> &cb, int timeout, int repeated)
{
    if (!p_event_loop || !cb || timeout <= 0)
    {
        return MError::Invalid;
    }
    MError err = this->MTimerEventBase::Init(p_event_loop);
    if (err != MError::No)
    {
        return err;
    }
    p_event_loop_ = p_event_loop;
    cb_ = cb;
    timeout_ = timeout;
    repeated_ = repeated;
    return MError::No;
}

void MTimeoutEvent::Clear()
{
    this->MTimerEventBase::Clear();
}

MError MTimeoutEvent::EnableEvent()
{

}

MError MTimeoutEvent::DisableEvent()
{

}
