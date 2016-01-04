#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_socket.h>
#include <net/m_net_event.h>

class MNetListener;
class MNetEventHandler;

class MNetConnector
{
public:
    MNetConnector();
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
    MSocket& GetSocket();
    MNetEvent& GetEvent();
    void SetListener(MNetListener *p_listener);
    MNetListener* GetListener();
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler& GetEventHandler();

    MNetError Init();

    MNetError AsyncRead(char *p_buf, size_t size, std::function<void ()> read_cb);
    MNetError AsyncWrite(const void *p_buf, size_t size, std::function<void ()> write_cb);
private:
    MSocket sock_;
    MNetEvent event_;
    MNetListener *p_listener_;
    MNetEventHandler &event_handler_;
};

#endif


