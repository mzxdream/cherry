#ifndef _M_EVENT_LOOP_H_
#define _M_EVENT_LOOP_H_

#include <map>
#include <vector>
#include <list>
#include <util/m_errno.h>
#include <util/m_type_define.h>
#include <sys/epoll.h>
#include <functional>

#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

#define MIOEVENT_IN      EPOLLIN
#define MIOEVENT_OUT     EPOLLOUT
#define MIOEVENT_ET      EPOLLET
#define MIOEVENT_RDHUP   EPOLLRDHUP
#define MIOEVENT_ERR     EPOLLERR
#define MIOEVENT_HUP     EPOLLHUP

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
    MError DelBeforeEvent(const MBeforeEventLocation &location);

    std::pair<MAfterEventLocation, MError> AddAfterEvent(const std::function<void ()> &cb);
    MError DelAfterEvent(const MAfterEventLocation &location);

    MError Interrupt();
    MError DispatchEvents(int timeout = -1);
private:
    MError AddInterrupt();
    MError DispatchIOEvents(bool forever, int64_t outdate_time);
    MError DispatchTimerEvents();
    MError DispatchBeforeEvents();
    MError DispatchAfterEvents();
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
