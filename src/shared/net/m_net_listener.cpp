#include <net/m_net_listener.h>
#include <net/m_socket.h>
#include <net/m_net_event_handler.h>

MNetListener::MNetListener(MSocket *p_sock, MNetEventHandler *p_event_handler)
    :p_sock_(p_sock)
    ,event_(p_sock, p_event_handler)
    ,p_event_handler_(p_event_handler)
    ,p_accept_sock_(nullptr)
    ,accept_cb_(nullptr)
    ,accept_actived_(false)
{
    event_.SetReadCallback(std::bind(&MNetListener::OnAccecptCallback, this));
    event_.SetErrorCallback(std::bind(&MNetListener::OnErrorCallback, this, std::placeholders::_1));
}

MNetListener::~MNetListener()
{
}

void MNetListener::SetSocket(MSocket *p_sock)
{
    p_sock_ = p_sock;
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
}

MNetEventHandler* MNetListener::GetEventHandler()
{
    return p_event_handler_;
}

MNetError MNetListener::EnableAccept()
{
    return event_.EnableRead(true);
}

MNetError MNetListener::AsyncAccept(MSocket *p_sock, std::function<void (MNetError)> accept_cb)
{
    if (!p_sock)
    {
        return MNetError::SockInvalid;
    }
    if (accept_actived_)
    {
        MNetError err = p_sock_->Accept(*p_sock);
        if (err )
    }
    else
    {
    }
}

void MNetListener::OnAccecptCallback()
{

}

void MNetListener::OnErrorCallback(MNetError err)
{
}
