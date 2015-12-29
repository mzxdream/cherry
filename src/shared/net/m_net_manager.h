#ifndef _M_NET_MANAGER_H_
#define _M_NET_MANAGER_H_

#include <net/m_net_event_handler.h>

class MNetManager
{
public:
    MNetManager();
    ~MNetManager();
    MNetManager(const MNetManager &) = delete;
    MNetManager& operator=(const MNetManager &) = delete;
public:
};

#endif
