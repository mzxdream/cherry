#ifndef _M_ERRNO_H_
#define _M_ERRNO_H_

enum class MError
{
    No = 0,
    Unknown = 1,
    ConnectOpened = 2,
    OutOfMemory = 3,
    SetParamFailed = 4,
    ConnectFailed = 5,
    Disconnect = 6,
    QueryFailed = 7,
    GetResFailed = 8,
    GetRowFailed = 9,
    GetParamFailed = 10,
    TypeNotSupport = 11,
    NoData = 12,
    ParamCountNotMatch = 13,
    ConvertFailed = 14,
};

#endif
