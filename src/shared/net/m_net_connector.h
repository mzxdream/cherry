#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_socket.h>
#include <net/m_net_event.h>

class MNetListener;
class MNetEventHandler;

class MNetConnector
{
public:
    MNetConnector(int sock, MNetListener &listener, MNetEventHandler &event_handler);
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
    MSocket& GetSocket();
    MNetEvent& GetEvent();
    MNetListener& GetListener();
    MNetEventHandler& GetEventHandler();

    MNetError Init();
private:
    MSocket sock_;
    MNetEvent event_;
    MNetListener &listener_;
    MNetEventHandler &event_handler_;
};

#endif


