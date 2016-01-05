#include <net/m_net_event.h>
#include <net/m_socket.h>
#include <net/m_net_event_handler.h>

MNetEvent::MNetEvent(MSocket *p_sock, MNetEventHandler *p_event_handler
    , std::function<void ()> read_cb, std::function<void ()> write_cb, std::function<void (MNetError)> error_cb)
    :p_sock_(p_sock)
    ,p_event_handler_(p_event_handler)
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

void MNetEvent::SetSocket(MSocket *p_sock)
{
    p_sock_ = p_sock;
}

MSocket* MNetEvent::GetSocket()
{
    return p_sock_;
}

void MNetEvent::SetEventHandler(MNetEventHandler *p_event_handler)
{
    p_event_handler_ = p_event_handler;
}

MNetEventHandler* MNetEvent::GetEventHandler()
{
    return p_event_handler_;
}

void MNetEvent::SetReadCallback(std::function<void ()> read_cb)
{
    read_cb_ = read_cb;
}

std::function<void ()>& MNetEvent::GetReadCallback()
{
    return read_cb_;
}

void MNetEvent::SetWriteCallback(std::function<void ()> write_cb)
{
    write_cb_ = write_cb;
}

std::function<void ()>* MNetEvent::GetWriteCallback()
{
    return write_cb_;
}

void MNetEvent::SetErrorCallback(std::function<void (MNetError)> error_cb)
{
    error_cb_ = error_cb;
}

std::function<void (MNetError)>& MNetEvent::GetErrorCallback()
{
    return error_cb_;
}

MNetError MNetEvent::EnableEvents(int events)
{
    if (!p_sock_)
    {
        return MNetError::SockInvalid;
    }
    if (!p_event_handler_)
    {
        return MNetError::EventHandlerInvalid;
    }
    if (events_actived_)
    {
        MNetError err = p_event_handler_->ModEvent(p_sock_->GetHandler(), events, this);
        if (err != MNetError::No)
        {
            return err;
        }
    }
    else
    {
        MNetError err = p_event_handler_->AddEvent(p_sock_->GetHandler(), events, this);
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
        if (!p_sock_)
        {
            return MNetError::SockInvalid;
        }
        MNetError err = p_event_handler_->DelEvent(p_sock_->GetHandler());
        if (err != MNetError::No)
        {
            return err;
        }
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
    if (error_cb_)
    {
        error_cb_(err);
    }
}
