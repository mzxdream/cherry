#ifndef _M_EVENT_LOOP_H_
#define _M_EVENT_LOOP_H_

#include <map>
#include <vector>
#include <list>
#include <util/m_errno.h>
#include <util/m_type_define.h>
#include <sys/epoll.h>

class MEventLoop
{
public:
    explicit MEventLoop();
    virtual ~MEventLoop();
    MEventLoop(const MEventLoop &) = delete;
    MEventLoop& operator=(const MEventLoop &) = delete;
public:
    MError Init();
    void Clear();

    int64_t GetTime() const;
    unsigned GetIOEventCount() const;
    unsigned GetAllEventCount() const;

    MError AddIOEvent(MIOEvent *p_event);
    MError ModIOEvent(MIOEvent *p_event);
    MError DelIOEvent(MIOEvent *p_event);

    MError AddTimerEvent(MTimerEvent *p_event);
    MError DelTimerEvent(MTimerEvent *p_event);

    MError AddCircleEventBeforeLoop(MCircleEvent *p_event);
    MError DelCircleEventBeforeLoop(MCircleEvent *p_event);

    MError AddCircleEventAfterLoop(MCircleEvent *p_event);
    MError DelCircleEventAfterLoop(MCircleEvent *p_event);

    MError Interrupt();//thread safe

    MError DispatchEvents(int timeout = -1);
private:
    MError AddInterrupt();
    int64_t GetLeastTime();
    MError DispatchIOEvents(int timeout);
    MError DispatchTimerEvents();
    MError DispatchCircleEventsBeforeLoop();
    MError DispatchCircleEventsAfterLoop();
private:
    int epoll_fd_;
    int64_t cur_time_;//milliseconds
    int interrupter_[2];
    std::vector<epoll_event> io_event_list_;
    unsigned io_event_count_;
    std::multimap<int64_t, MTimerEvent*> timer_events_;
    std::list<MCircleEvent*> circle_before_events_;
    std::list<MCircleEvent*> circle_after_events_;
};

#endif
