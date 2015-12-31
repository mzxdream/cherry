#ifndef _M_SESSION_MANAGER_H_
#define _M_SESSION_MANAGER_H_

#include <net/m_net_manager.h>

class MSessionManager
{
public:
    explicit MSessionManager(size_t net_count);
    ~MSessionManager();
    MSessionManager(const MSessionManager &) = delete;
    MSessionManager& operator=(const MSessionManager &) = delete;
public:
    bool Init();
    void Close();
private:
    MNetManager net_manager;
};

#endif
