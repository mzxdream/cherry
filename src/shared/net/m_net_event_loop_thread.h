#ifndef _M_NET_EVENT_LOOP_THREAD_H_
#define _M_NET_EVENT_LOOP_THREAD_H_

#include <net/m_net_event_loop.h>
#include <thread/m_thread.h>
#include <mutex>
#include <functional>
#include <list>

class MNetEventLoopThread
    :public MThread
{
public:
    MNetEventLoopThread(size_t single_process_events = 128);
    ~MNetEventLoopThread();
    MNetEventLoopThread(const MNetEventLoopThread &) = delete;
    MNetEventLoopThread& operator=(const MNetEventLoopThread &) = delete;
public:
    MError Init();
    MError Close();
    MNetEventLoop& GetEventLoop();
    size_t GetEventCount() const;
    void AddCallback(const std::function<void ()> &cb);
    MError Interrupt();
private:
    virtual void DoRun() override;
private:
    MNetEventLoop event_loop_;
    std::list<std::function<void ()> > cb_list_;
    std::mutex cb_mutex_;
};

#endif
