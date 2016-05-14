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
    MError Init(MEventLoop *p_event_loop, int fd);
    void Clear();
    MError EnableEvent(unsigned event);
    MError DisableEvent(unsigned event);
    MError DisableEvent();

    void OnCallback(unsigned event);
public:
    virtual void _OnCallback(unsigned event) = 0;
private:
    MEventLoop *p_event_loop_;
    int fd_;
    unsigned event_;
    bool event_actived_;
};

class MTimerEventBase
{
public:
    MTimerEventBase();
    virtual ~MTimerEventBase();
    MTimerEventBase(const MTimerEventBase &) = delete;
    MTimerEventBase& operator=(const MTimerEventBase &) = delete;
public:
    MError Init(MEventLoop *p_event_loop);
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    void OnCallback();
public:
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
};

class MBeforeEventBase
{
public:
    MBeforeEventBase();
    virtual ~MBeforeEventBase();
    MBeforeEventBase(const MBeforeEventBase &) = delete;
    MBeforeEventBase& operator=(const MBeforeEventBase &) = delete;
public:
    MError Init();
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    bool IsNeedRepeated();
    void OnCallback();
public:
    virtual bool _IsNeedRepeated() = 0;
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
};

class MAfterEventBase
{
public:
    MAfterEventBase();
    virtual ~MAfterEventBase();
    MAfterEventBase(const MAfterEventBase &) = delete;
    MAfterEventBase& operator=(const MAfterEventBase &) = delete;
public:
    MError Init();
    void Clear();
    MError EnableEvent();
    MError DisableEvent();

    bool IsNeedRepeated();
    void OnCallback();
public:
    virtual bool _IsNeedRepeated() = 0;
    virtual void _OnCallback() = 0;
private:
    MEventLoop *p_event_loop_;
};

#endif
