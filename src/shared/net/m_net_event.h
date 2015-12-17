#ifndef _M_NET_EVENT_H_
#define _M_NET_EVENT_H_

#include <functional>
#include <sys/epoll.h>

#define M_NET_EVENT_READ (EPOLLIN|EPOLLRDHUP)
#define M_NET_EVENT_WRITE EPOLLOUT
#define M_NET_EVENT_LEVEL 0
#define M_NET_EVENT_EDGE EPOLLET

enum MNetEventError
{
    No = 0,
    Unknown = 1,
};

class MNetEvent
{
public:
    MNetEvent();
    ~MNetEvent();
public:
    void SetActive(bool active);
    bool GetActive() const;
    void SetReadCallback(std::function<void ()> cb_read);
    void SetWriteCallback(std::function<void ()> cb_write);
    void SetCloseCallback(std::function<void ()> cb_close);
    void SetErrorCallback(std::function<void ()> cb_error);

    void OnRead();
    void OnWrite();
    void OnClose();
    void OnError(MNetEventError err);
private:
    bool active_;
    std::function<void ()> cb_read_;
    std::function<void ()> cb_write_;
    std::function<void ()> cb_close_;
    std::function<void (MNetEventError)> cb_error_;
};

#endif
