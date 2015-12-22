#ifndef _M_NET_HANDLER_H_
#define _M_NET_HANDLER_H_

#include <net/m_epoll.h>
#include <thread/m_thread.h>
#include <net/m_net_listener.h>
#include <net/m_net_connector.h>
#include <net/m_net_peer_connector.h>

class MNetHandler
    :public MThread
{
public:
    MNetHandler();
    virtual ~MNetHandler();
    MNetHandler(const MNetHandler &) = delete;
    MNetHandler& operator=(const MNetHandler &) = delete;
public:
    MNetError Create(size_t max_events);
    MNetError Close();
    MNetError AddListener(MNetListener *p_listener);
    MNetError AddConnector(MNetConnector *p_connector);
    MNetError AddPeerConnector(MNetPeerConnector *p_peer_connector);
private:
    virtual void DoRun() override;
private:
    MEpoll handler_;
};

#endif
