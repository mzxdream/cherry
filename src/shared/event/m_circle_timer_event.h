#ifndef _M_TIMER_EVENT_H_
#define _M_TIMER_EVENT_H_

#include <util/m_type_define.h>
#include <util/m_errno.h>
#include <map>
#include <functional>

class MEventLoop;

class MTimerEvent
{
public:
    MTimerEvent();
    ~MTimerEvent();
    MTimerEvent(const MTimerEvent &) = delete;
    MTimerEvent& operator=(const MTimerEvent &) = delete;
public:
    void SetLoopLocation(std::multimap<int64_t, MTimerEvent*>::iterator location);
    std::multimap<int64_t, MTimerEvent*>::iterator GetLoopLocation() const;
    void SetEventActive(bool active);
    bool IsEventActive() const;
    int64_t GetStartTime() const;
    int GetTimeout() const;
    bool NeedRepeat() const;

    MError EnableEvents(MEventLoop *p_event_loop, const std::function<void ()> &cb
        , int timeout, int repeat_count, int64_t start_time);
    MError EnableEvents(MEventLoop *p_event_loop, const std::function<void ()> &cb
        , int timeout, int repeat_count = 0);
    MError DisableEvents();
public:
    void OnCallback();
private:
    bool event_active_;
    std::multimap<int64_t, MTimerEvent*>::iterator loop_location_;
    MEventLoop *p_event_loop_;
    int64_t start_time_;
    int timeout_;
    int repeat_count_;
    std::function<void ()> cb_;
};

#endif
