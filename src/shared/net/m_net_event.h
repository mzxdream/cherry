#ifndef _M_NET_EVENT_H_
#define _M_NET_EVENT_H_

#include <functional>
#include <sys/epoll.h>

#define M_NET_EVENT_READ (EPOLLIN|EPOLLRDHUP)
#define M_NET_EVENT_WRITE EPOLLOUT
#define M_NET_EVENT_LEVEL 0
#define M_NET_EVENT_EDGE EPOLLET

class MNetEvent
{
public:
    MNetEvent();
    ~MNetEvent();
public:
    void SetEvent(int event);
    int  GetEvent() const;
    void SetReadCallback(const std::function<void ()> &cb_read);
    void SetWriteCallback(const std::function<void ()> &cb_write);
    void SetCloseCallback(const std::function<void ()> &cb_close);

    void OnRead();
    void OnWrite();
    void OnClose();
private:
    int event_;
    std::function<void ()> cb_read_;
    std::function<void ()> cb_write_;
    std::function<void ()> cb_close_;
};

#endif
