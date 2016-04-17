#ifndef _M_TIMER_EVENT_H_
#define _M_TIMER_EVENT_H_

class MTimerEvent
{
public:
    MTimerEvent();
    virtual ~MTimerEvent();
public:
    void SetLoopLocation(std::multimap<int64_t, MTimerEvent*>::iterator);
    const std::multimap<int64_t, MTimerEvent*>::iterator& GetLoopLocation() const;

    virtual void GetStartTime() = 0;
    virtual void OnCallback() = 0;
private:
    std::multimap<int64_t, MTimerEvent*>::iterator loop_location_;
};

#endif
