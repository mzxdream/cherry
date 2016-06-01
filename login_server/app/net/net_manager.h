#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <mzx/util/m_type_define.h>
#include <vector>
#include "net_worker.h"

class NetManager
{
public:
    NetManager();
    ~NetManager();
    NetManager(const NetManager &) = delete;
    NetManager& operator=(const NetManager &) = delete;
public:
    MError Init();
    void Clear();
private:
    std::vector<NetWorker> workers_;
};

#endif
