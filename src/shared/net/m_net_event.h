#ifndef _M_NET_EVENT_H_
#define _M_NET_EVENT_H_

#include <net/m_net_common.h>
#include <functional>

class MNetEventHandler;
class MSocket;

class MNetEvent
{
public:
    explicit MNetEvent(MSocket *p_sock, MNetEventHandler *p_event_handler
        , std::function<void ()> read_cb, std::function<void ()> write_cb, std::function<void (MNetError)> error_cb);
    ~MNetEvent();
    MNetEvent(const MNetEvent &) = delete;
    MNetEvent& operator=(const MNetEvent &) = delete;
public:
    void SetSocket(MSocket *p_sock);
    MSocket* GetSocket();
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler* GetEventHandler();
    void SetReadCallback(std::function<void ()> read_cb);
    std::function<void ()>& GetReadCallback();
    void SetWriteCallback(std::function<void ()> write_cb);
    std::function<void ()>& GetWriteCallback();
    void SetErrorCallback(std::function<void (MNetError)> error_cb);
    std::function<void (MNetError)>& GetErrorCallback();

    MNetError EnableEvents(int events);
    MNetError DisableEvents();
public:
    void OnReadCallback();
    void OnWriteCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket *p_sock_;
    MNetEventHandler *p_event_handler_;
    std::function<void ()> read_cb_;
    std::function<void ()> write_cb_;
    std::function<void (MNetError)> error_cb_;
    bool events_actived_;
};

#endif
