#ifndef _M_NET_COMMON_H_
#define _M_NET_COMMON_H_

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
    EventHandlerInvalid = 9,
};

#endif
