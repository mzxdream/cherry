#include <net/m_net_connector.h>
#include <net/m_net_listener.h>
#include <net/m_net_event_handler.h>

MNetConnector::MNetConnector(int sock, MNetListener &listener, MNetEventHandler &event_handler)
    :sock_(sock)
    ,event_(sock_, event_handler)
    ,listener_(listener)
    ,event_handler_(event_handler)
{
}

MNetConnector::~MNetConnector()
{
}

MSocket& MNetConnector::GetSocket()
{
    return sock_;
}

MNetEvent& MNetConnector::GetEvent()
{
    return event_;
}

MNetListener& MNetConnector::GetListener()
{
    return listener_;
}

MNetEventHandler& MNetConnector::GetEventHandler()
{
    return event_handler_;
}

MNetError MNetConnector::Init()
{
    MNetError err = sock_.SetBlock(false);
    if (err != MNetError::No)
    {
        return err;
    }
    return MNetError::No;
}
