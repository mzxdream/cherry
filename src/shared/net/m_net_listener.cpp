#include <net/m_net_listener.h>
#include <net/m_socket.h>
#include <net/m_net_event_handler.h>

MNetListener::MNetListener(MSocket &sock, MNetEventHandler &event_handler)
    :sock_(sock)
    ,event_(sock, event_handler)
    ,event_handler_(event_handler)
{
}

MNetListener::~MNetListener()
{
}

MSocket& MNetListener::GetSocket()
{
    return sock_;
}

MNetEvent& MNetListener::GetEvent()
{
    return event_;
}

MNetEventHandler& MNetListener::GetEventHandler()
{
    return event_handler_;
}
