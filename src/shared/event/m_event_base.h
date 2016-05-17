#ifndef _M_EVENT_BASE_H_
#define _M_EVENT_BASE_H_

#include <util/m_errno.h>
#include <sys/epoll.h>
#include <util/m_type_define.h>

#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

#define MIOEVENT_IN      EPOLLIN
#define MIOEVENT_OUT     EPOLLOUT
#define MIOEVENT_ET      EPOLLET
#define MIOEVENT_RDHUP   EPOLLRDHUP
#define MIOEVENT_ERR     EPOLLERR
#define MIOEVENT_HUP     EPOLLHUP
#define MIOEVENT_ONESHOT EPOLLONESHOT

class MEventLoop;

class MIOEventBase
{
public:
    MIOEventBase();
    virtual ~MIOEventBase();
    MIOEventBase(const MIOEventBase &) = delete;
    MIOEventBase& operator=(const MIOEventBase &) = delete;
public:
    int GetFD() const;
    unsigned GetEvents() const;
    void SetActived(bool actived);
    bool IsActived() const;

    MError Init(MEventLoop *p_event_loop, int fd);
    void Clear();
    MError EnableEvent(unsigned events);
    MError DisableEvent(unsigned events);
    MError DisableEvent();

    void OnCallback(unsigned events);
public:
    virtual void _OnCallback(unsigned events) = 0;
private:
    MEventLoop *p_event_loop_;
    int fd_;
    unsigned events_;
    bool actived_;
};

class MTimerEventBase
{
    typedef std::multimap<int64_t, MTimerEventBase*>::iterator MTimerEventLocation;
public:
    MTimerEventBase();
    virtual ~MTimerEventBase();
    MTimerEventBase(const MTimerEventBase &) = delete;
    MTimerEventBase& operator=(const MTimerEventBase &) = delete;
public:
    int64_t GetStartTime();
    void SetActived(bool actived);
    bool IsActived() const;
    void SetLocation(MTimerEventLocation location);
    MTimerEventLocation GetLocation() const;

    MError Init(MEventLoop *p_event_loop);
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    void OnCallback();
public:
    virtual int64_t _GetStartTime() = 0;
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
    bool actived_;
    MTimerEventLocation location_;
};

class MBeforeEventBase
{
    typedef std::list<MBeforeEventBase*>::iterator MBeforeEventLocation;
public:
    MBeforeEventBase();
    virtual ~MBeforeEventBase();
    MBeforeEventBase(const MBeforeEventBase &) = delete;
    MBeforeEventBase& operator=(const MBeforeEventBase &) = delete;
public:
    void SetActived(bool actived);
    bool IsActived() const;
    void SetLocation(MBeforeEventLocation location);
    MBeforeEventLocation GetLocation() const;

    MError Init(MEventLoop *p_event_loop);
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    void OnCallback();
public:
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
    bool actived_;
    MBeforeEventLocation location_;
};

class MAfterEventBase
{
    typedef std::list<MAfterEventBase*>::iterator MAfterEventLocation;
public:
    MAfterEventBase();
    virtual ~MAfterEventBase();
    MAfterEventBase(const MAfterEventBase &) = delete;
    MAfterEventBase& operator=(const MAfterEventBase &) = delete;
public:
    void SetActived(bool actived);
    bool IsActived() const;
    void SetLocation(MAfterEventLocation location);
    MAfterEventLocation GetLocation() const;

    MError Init();
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    void OnCallback();
public:
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
    bool actived_;
    MAfterEventLocation location_;
};

#endif
