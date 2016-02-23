#ifndef _NET_SESSION_H_
#define _NET_SESSION_H_

#include <net/m_net_connector.h>
#include <net/m_net_event_loop_thread.h>
#include <util/m_type_define.h>
#include <mutex>

struct NetPackage
{
    uint32_t cmd;
    char *p_content;
    uint16_t content_len;
};

class NetSession
{
public:
    NetSession();
    ~NetSession();
    NetSession(const NetSession &) = delete;
    NetSession& operator=(const NetSession &) = delete;
public:
    void Update();
private:
    MNetConnector *p_connector_;
    MNetEventLoopThread *p_loop_thread_;
    uint16_t pkg_len_;
    bool pkg_len_read_;
    std::mutex pkg_mtx_;
    std::list<NetPackage> pkg_list;
};

#endif
