#include <event/m_event_loop.h>
#include <event/m_event_base.h>

MIOEventBase::MIOEventBase()
    :p_event_loop_(nullptr)
    ,fd_(-1)
    ,event_(0)
    ,event_actived_(false)
{
}

MIOEventBase::~MIOEventBase()
{
    Clear();
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
    return MError::No;
}

void MIOEventBase::Clear()
{
    DisableEvent();
}

MError MIOEventBase::EnableEvent(unsigned event)
{
    return MError::No;
}

MError MIOEventBase::DisableEvent(unsigned event)
{
    return MError::No;
}

MError MIOEventBase::DisableEvent()
{
    return MError::No;
}

void MIOEventBase::OnCallback(unsigned event)
{
}

MTimerEventBase::MTimerEventBase()
    :p_event_loop_(nullptr)
{
}


