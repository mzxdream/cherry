#ifndef _M_NET_EVENT_H_
#define _M_NET_EVENT_H_

#include <net/m_net_common.h>
#include <functional>

class MNetEventHandler;
class MSocket;

class MNetEvent
{
public:
    MNetEvent(MSocket &sock, MNetEventHandler &event_handler);
    ~MNetEvent();
public:
    MNetError EnableRead(bool enable);
    MNetError EnableWrite(bool enable);
    void SetReadCallback(std::function<void ()> read_cb);
    void SetWriteCallback(std::function<void ()> write_cb);
    void SetErrorCallback(std::function<void (MNetError)> error_cb);
public:
    void OnReadCallback();
    void OnWriteCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket &sock_;
    MNetEventHandler &event_handler_;
    bool enable_read_;
    bool enable_write_;
    std::function<void ()> read_cb_;
    std::function<void ()> write_cb_;
    std::function<void (MNetError)> error_cb_;
};

#endif
