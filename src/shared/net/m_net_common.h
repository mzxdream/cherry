#ifndef _M_NET_COMMON_H_
#define _M_NET_COMMON_H_

#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <cstddef>

#define M_NET_EVENT_READ (EPOLLIN|EPOLLRDHUP)
#define M_NET_EVENT_WRITE EPOLLOUT
#define M_NET_EVENT_LEVEL 0
#define M_NET_EVENT_EDGE EPOLLET

enum class MNetError
{
    No = 0,
    Unknown = 1,
    Disconnect = 2,
    Created = 3,
    InProgress = 4,
    Again = 5,
    InterruptedSysCall = 6,
    OutOfMemory = 7,
    SockInvalid = 8,
    EventLoopInvalid = 9,
    BufferInsufficient = 10,
    BufferOverflow = 11,
};

inline MNetError MNetCheckLastError()
{
    if (errno == EINPROGRESS)
    {
        return MNetError::InProgress;
    }
    else if (errno == EAGAIN)
    {
        return MNetError::Again;
    }
    else if (errno == EINTR)
    {
        return MNetError::InterruptedSysCall;
    }
    return MNetError::Unknown;
}

#endif
