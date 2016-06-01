#ifndef _NET_SESSION_H_
#define _NET_SESSION_H_

#include <mzx/util/m_type_define.h>

class NetSession
{
public:
private:
    unsigned id_;
    int fd_;
    char *p_buf_;
};

#endif
