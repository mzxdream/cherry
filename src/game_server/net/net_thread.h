#ifndef _NET_THREAD_H_
#define _NET_THREAD_H_

#include <net/m_net_event_loop.h>
#include <thread/m_thread.h>
#include <mutex>
#include <functional>
#include <list>

class NetManager;

class NetThread
    :public MThread
{
public:
    NetThread();
    ~NetThread();
    NetThread(const NetThread &) = delete;
    NetThread& operator=(const NetThread &) = delete;
public:
    MNetEventLoop& GetEventLoop();
    void AddCallback(const std::function<void ()> &callback);
private:
    virtual void DoRun();
private:
    MNetEventLoop event_loop_;
    std::list<std::function<void ()> > callback_list_;
    std::mutex callback_mutex_;
};

#endif
