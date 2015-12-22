#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_socket.h>
#include <net/m_net_event.h>

class MNetListener;

class MNetConnector
{
public:
    MNetConnector();
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
    void SetListener(MNetListener *p_listener);
    MSocket& GetSocket();
    MNetEvent& GetEvent();
private:
    MSocket sock_;
    MNetListener *p_listener_;
    MNetEvent event_;
};

#endif


