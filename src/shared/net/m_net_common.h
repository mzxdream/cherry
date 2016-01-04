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
    OutOfMemory = 6,
    SockInvalid = 7,
    EventHandlerInvalid = 8,
};

#endif
