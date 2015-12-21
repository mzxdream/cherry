#ifndef _M_NET_MANAGER_H_
#define _M_NET_MANAGER_H_

#include <net/m_epoll.h>
#include <vector>

#define M_NET_HANDLERS 4
#define M_NET_EVENTS 100

class MNetManager
{
public:
    MNetManager();
    ~MNetManager();
    MNetManager(const MNetManager &) = delete;
    MNetManager& operator=(const MNetManager &) = delete;
private:
    MEpoll handler_list_[M_NET_HANDLERS];
};

#endif
