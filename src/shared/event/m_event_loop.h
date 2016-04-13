#ifndef _M_EVENT_LOOP_H_
#define _M_EVENT_LOOP_H_

#include <map>
#include <vector>
#include <list>
#include <util/m_errno.h>
#include <util/m_type_define.h>
#include <sys/epoll.h>

#define IOEVENT_IN 1
#define IOEVENT_OUT 2

class MEventLoop
{
public:
    MEventLoop();
    ~MEventLoop();
    MEventLoop(const MEventLoop &) = delete;
    MEventLoop& operator=(const MEventLoop &) = delete;
public:
    MError Init();
    void Clear();

    int64_t GetTime() const;
    void UpdateTime();
    unsigned GetIOEventCount() const;
    unsigned GetAllEventCount() const;

    MError AddIOEvent(MIOEvent *p_event);
    MError ModIOEvent(MIOEvent *p_event);
    MError DelIOEvent(MIOEvent *p_event);

    MError AddTimerEvent(MTimerEvent *p_event);
    MError DelTimerEvent(MTimerEvent *p_event);

    MError AddBeforeIdleEvent(MIdleEvent *p_event);
    MError DelBeforeIdleEvent(MIdleEvent *p_event);

    MError AddAfterIdleEvent(MIdleEvent *p_event);
    MError DelAfterIdleEvent(MIdleEvent *p_event);

    MError Interrupt();//thread safe

    MError DispatchEvents(int timeout = -1);
private:
    MError AddInterrupt();
    MError DispatchIOEvents(bool forever, int64_t outdate_time);
    MError DispatchTimerEvents();
    MError DispatchBeforeIdleEvents();
    MError DispatchAfterIdleEvents();
private:
    int epoll_fd_;
    int64_t cur_time_;//milliseconds
    int interrupter_[2];
    std::vector<epoll_event> io_event_list_;
    unsigned io_event_count_;
    std::multimap<int64_t, MTimerEvent*> timer_events_;
    std::list<MIdleEvent*> before_idle_events_;
    std::list<MIdleEvent*> after_idle_events_;
};

#endif
