#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <util/m_singleton.h>
#include <map>
#include <net_session.h>
#include <net/m_net_event_loop_thread.h>

class NetManager
    :public MSingleton<NetManager>
{
public:
    NetManager();
    ~NetManager();
    NetManager(const NetManager &) = delete;
    NetManager& operator=(const NetManager &) = delete;
public:
    bool Init();
private:
    std::vector<
    std::map<int64, NetSession*> sessions_;
};

#endif
