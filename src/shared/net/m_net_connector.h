#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_net_event.h>
#include <list>
#include <tuple>

class MSocket;
class MNetListener;
class MNetEventHandler;

class MNetConnector
{
public:
    MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventHandler *p_event_handler
        , std::function<void (const char*, size_t)> read_cb, std::function<void (MNetError)> error_cb, bool need_free_sock);
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
    void SetSocket(MSocket *p_sock);
    MSocket* GetSocket();
    MNetEvent& GetEvent();
    void SetListener(MNetListener *p_listener);
    MNetListener* GetListener();
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler* GetEventHandler();
    void SetReadCallback(std::function<void (const char*, size_t)> read_cb);
    std::function<void (const char*, size_t)>& GetReadCallback();
    void SetErrorCallback(std::function<void (MNetError)> error_cb);
    std::function<void (MNetError)>& GetErrorCallback();
    void SetNeedFreeSock(bool need);
    bool GetNeedFreeSock() const;



private:
    MSocket *p_sock_;
    MNetEvent event_;
    MNetListener *p_listener_;
    MNetEventHandler *p_event_handler_;
    std::function<void (const void*, size_t)> read_cb_;
    std::function<void (MNetError)> error_cb_;
    bool need_free_sock_;
    char *p_read_buffer_;
    size_t read_len_;
    std::list<std::tuple<> >
};

#endif


