#include <net/m_net_listener.h>
#include <net/m_socket.h>
#include <net/m_net_event_handler.h>

#define M_NET_MULIT_ACCEPT_COUNT 5

MNetListener::MNetListener(MSocket *p_sock, MNetEventHandler *p_event_handler
    , std::function<void (MSocket*)> accept_cb, std::function<void (MNetError)> error_cb, bool need_free_sock)
    :p_sock_(p_sock)
    ,event_(p_sock, p_event_handler, nullptr, nullptr, nullptr)
    ,p_event_handler_(p_event_handler)
    ,accept_cb_(accept_cb)
    ,error_cb_(error_cb)
    ,need_free_sock_(need_free_sock)
{
    event_.SetReadCallback(std::bind(&MNetListener::OnAcceptCallback, this));
    event_.SetErrorCallback(std::bind(&MNetListener::OnErrorCallback, this, std::placeholders::_1));
}

MNetListener::~MNetListener()
{
    if (need_free_sock_ && p_sock_)
    {
        delete p_sock_;
    }
}

void MNetListener::SetSocket(MSocket *p_sock)
{
    p_sock_ = p_sock;
    event_.SetSocket(p_sock_);
}

MSocket* MNetListener::GetSocket()
{
    return p_sock_;
}

MNetEvent& MNetListener::GetEvent()
{
    return event_;
}

void MNetListener::SetEventHandler(MNetEventHandler *p_event_handler)
{
    p_event_handler_ = p_event_handler;
    event_.SetEventHandler(p_event_handler_);
}

MNetEventHandler* MNetListener::GetEventHandler()
{
    return p_event_handler_;
}

void MNetListener::SetAcceptCallback(std::function<void (MSocket*)> accept_cb)
{
    accept_cb_ = accept_cb;
}

std::function<void (MSocket*)>& MNetListener::GetAcceptCallback()
{
    return accept_cb_;
}

void MNetListener::SetErrorCallback(std::function<void (MNetError)> error_cb)
{
    error_cb_ = error_cb;
}

std::function<void (MNetError)>& MNetListener::GetErrorCallback()
{
    return error_cb_;
}

void MNetListener::SetNeedFreeSock(bool need)
{
    need_free_sock_ = need;
}

bool MNetListener::GetNeedFreeSock() const
{
    return need_free_sock_;
}

MNetError MNetListener::EnableAccept(bool enable)
{
    if (enable)
    {
        return event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_LEVEL);
    }
    else
    {
        return event_.DisableEvents();
    }
}

void MNetListener::OnAcceptCallback()
{
    int multi_accept_count = M_NET_MULIT_ACCEPT_COUNT + 1;
    while (--multi_accept_count)
    {
        MSocket *p_conn_sock = new MSocket();
        if (!p_conn_sock)
        {
            OnErrorCallback(MNetError::OutOfMemory);
            return;
        }
        MNetError err = p_sock_->Accept(*p_conn_sock);
        if (err != MNetError::No)
        {
            delete p_conn_sock;
            if (err != MNetError::Again
                && err != MNetError::InterruptedSysCall)
            {
                OnErrorCallback(err);
            }
            return;
        }
    }
}

void MNetListener::OnErrorCallback(MNetError err)
{
    if (error_cb_)
    {
        error_cb_(err);
    }
}
