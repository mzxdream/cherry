#include <net/m_net_event.h>
#include <net/m_net_event_loop.h>

MNetEvent::MNetEvent(int fd, MNetEventLoop *p_event_loop
    , const std::function<void ()> &read_cb, const std::function<void ()> &write_cb, const std::function<void (MNetError)> &error_cb)
    :fd_(fd)
    ,p_event_loop_(p_event_loop)
    ,read_cb_(read_cb)
    ,write_cb_(write_cb)
    ,error_cb_(error_cb)
    ,events_actived_(false)
{
}

MNetEvent::~MNetEvent()
{
    DisableEvents();
}

void MNetEvent::SetFD(int fd)
{
    fd_ = fd;
}

int MNetEvent::GetFD() const
{
    return fd_;
}

void MNetEvent::SetEventLoop(MNetEventLoop *p_event_loop)
{
    p_event_loop_ = p_event_loop;
}

MNetEventLoop* MNetEvent::GetEventLoop()
{
    return p_event_loop_;
}

void MNetEvent::SetReadCallback(const std::function<void ()> &read_cb)
{
    read_cb_ = read_cb;
}

std::function<void ()>& MNetEvent::GetReadCallback()
{
    return read_cb_;
}

void MNetEvent::SetWriteCallback(const std::function<void ()> &write_cb)
{
    write_cb_ = write_cb;
}

std::function<void ()>& MNetEvent::GetWriteCallback()
{
    return write_cb_;
}

void MNetEvent::SetErrorCallback(const std::function<void (MNetError)> &error_cb)
{
    error_cb_ = error_cb;
}

std::function<void (MNetError)>& MNetEvent::GetErrorCallback()
{
    return error_cb_;
}

MNetError MNetEvent::EnableEvents(int events)
{
    if (!p_event_loop_)
    {
        return MNetError::EventLoopInvalid;
    }
    if (events_actived_)
    {
        MNetError err = p_event_loop_->ModEvent(fd_, events, this);
        if (err != MNetError::No)
        {
            return err;
        }
    }
    else
    {
        MNetError err = p_event_loop_->AddEvent(fd_, events, this);
        if (err != MNetError::No)
        {
            return err;
        }
        events_actived_ = true;
    }
    return MNetError::No;
}

MNetError MNetEvent::DisableEvents()
{
    if (events_actived_)
    {
        if (!p_event_loop_)
        {
            return MNetError::EventLoopInvalid;
        }
        MNetError err = p_event_loop_->DelEvent(fd_);
        if (err != MNetError::No)
        {
            return err;
        }
        events_actived_ = false;
    }
    return MNetError::No;
}

void MNetEvent::OnReadCallback()
{
    if (read_cb_)
    {
        read_cb_();
    }
}

void MNetEvent::OnWriteCallback()
{
    if (write_cb_)
    {
        write_cb_();
    }
}

void MNetEvent::OnErrorCallback(MNetError err)
{
    DisableEvents();
    if (error_cb_)
    {
        error_cb_(err);
    }
}
