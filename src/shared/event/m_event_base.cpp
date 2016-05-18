#include <event/m_event_loop.h>
#include <event/m_event_base.h>

MIOEventBase::MIOEventBase()
    :p_event_loop_(nullptr)
    ,fd_(-1)
    ,events_(0)
    ,actived_(false)
{
}

MIOEventBase::~MIOEventBase()
{
    Clear();
}

int MIOEventBase::GetFD() const
{
    return fd_;
}

void MIOEventBase::SetEvents(unsigned events)
{
    events_ = events;
}

unsigned MIOEventBase::GetEvents() const
{
    return events_;
}

void MIOEventBase::SetActived(bool actived)
{
    actived_ = actived;
}

bool MIOEventBase::IsActived() const
{
    return actived_;
}

MError MIOEventBase::Init(MEventLoop *p_event_loop, int fd)
{
    if (!p_event_loop)
    {
        return MError::Invalid;
    }
    if (fd < 0)
    {
        return MError::Invalid;
    }
    p_event_loop_ = p_event_loop;
    fd_ = fd;
    events_ = 0;
    actived_ = false;
    return MError::No;
}

void MIOEventBase::Clear()
{
    DisableAllEvent();
}

MError MIOEventBase::EnableEvent(unsigned events)
{
    return p_event_loop_->AddIOEvent(events, this);
}

MError MIOEventBase::DisableEvent(unsigned events)
{
    return p_event_loop_->DelIOEvent(events, this);
}

MError MIOEventBase::DisableAllEvent()
{
    return DisableEvent(MIOEVENT_IN | MIOEVENT_OUT | MIOEVENT_RDHUP);
}

void MIOEventBase::OnCallback(unsigned event)
{
}

MTimerEventBase::MTimerEventBase()
    :p_event_loop_(nullptr)
{
}


