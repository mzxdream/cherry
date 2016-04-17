#ifndef _M_EVENT_LOOP_H_
#define _M_EVENT_LOOP_H_

#include <map>
#include <vector>
#include <list>
#include <util/m_errno.h>
#include <util/m_type_define.h>
#include <sys/epoll.h>
#include <functional>

#define MIOEVENT_IN      0x01
#define MIOEVENT_OUT     0x02
#define MIOEVENT_RDHUP   0x04
#define MIOEVENT_ET      0x08
#define MIOEVENT_ERR     0x10
#define MIOEVENT_HUP     0x20

typedef std::multimap<int64_t, std::function<void ()> >::iterator MTimerEventLocation;
typedef std::list<std::function<void ()> >::iterator MBeforeEventLocation;
typedef std::list<std::function<void ()> >::iterator MAfterEventLocation;

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

    size_t GetIOEventCount() const;
    size_t GetTimerEventCount() const;
    size_t GetBeforeEventCount() const;
    size_t GetAfterEventCount() const;
    size_t GetAllEventCount() const;

    MError AddIOEvent(int fd, unsigned events, const std::function<void (unsigned)> &cb);
    MError DelIOEvent(int fd, unsigned events = (MIOEVENT_IN | MIOEVENT_OUT | MIOEVENT_RDHUP));

    std::pair<MTimerEventLocation, MError> AddTimerEvent(int64_t start_time, const std::function<void ()> &cb);
    MError DelTimerEvent(const MTimerEventLocation &location);

    std::pair<MBeforeEventLocation, MError> AddBeforeEvent(const std::function<void ()> &cb);
    MError DelBeforeEvent(const std::function<void ()> &cb);

    std::pair<MAfterEventLocation, MError> AddAfterEvent(const std::function<void ()> &cb);
    MError DelAfterEvent(const std::function<void ()> &cb);

    MError Interrupt();
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
    std::map<int, std::pair<unsigned, std::function<void (unsigned)> > > io_events_;
    std::multimap<int64_t, std::function<void ()> > timer_events_;
    std::list<std::function<void ()> > before_events_;
    std::list<std::function<void ()> > after_events_;
};

#endif
