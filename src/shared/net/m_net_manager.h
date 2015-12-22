#ifndef _M_NET_MANAGER_H_
#define _M_NET_MANAGER_H_

#include <net/m_epoll.h>
#include <vector>
#include <thread>

class MNetManager
{
public:
    MNetManager();
    ~MNetManager();
    MNetManager(const MNetManager &) = delete;
    MNetManager& operator=(const MNetManager &) = delete;
public:
    MNetError Create(int handler_count, int event_count);
    MNetError Start();
    MNetError Stop();
private:
    std::vector<MEpoll> handler_list_;
    std::vector<std::thread> thread_list_;
};

#endif
