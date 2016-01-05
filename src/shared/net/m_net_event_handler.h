#ifndef _M_NET_EVENT_HANDLER_H_
#define _M_NET_EVENT_HANDLER_H_

#include <sys/epoll.h>
#include <net/m_net_common.h>
#include <net/m_thread.h>

#define M_NET_EVENT_READ (EPOLLIN|EPOLLRDHUP)
#define M_NET_EVENT_WRITE EPOLLOUT
#define M_NET_EVENT_LEVEL 0
#define M_NET_EVENT_EDGE EPOLLET

#define M_NET_MAX_EVENTS 128

class MNetEvent;

class MNetEventHandler
    :public MThread
{
public:
    MNetEventHandler();
    virtual ~MNetEventHandler();
    MNetEventHandler(const MNetEventHandler &) = delete;
    MNetEventHandler& operator=(const MNetEventHandler &) = delete;
public:
    size_t GetEventCount() const;
    MNetError Create();
    MNetError Close();
    MNetError AddEvent(int fd, int events, MNetEvent *p_event);
    MNetError ModEvent(int fd, int events, MNetEvent *p_event);
    MNetError DelEvent(int fd);
    MNetError ProcessEvents();
    MNetError Interrupt();
private:
    virtual void DoRun() override;
private:
    MNetError CheckError();
private:
    int epoll_fd_;
    epoll_event event_list_[M_NET_MAX_EVENTS];
    int interrupter_[2];
    size_t event_count_;
};

#endif
