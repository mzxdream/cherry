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
    NetSession(MSocket *p_sock, MNetEventLoopThread *p_event_loop_thread);
    ~NetSession();
    NetSession(const NetSession &) = delete;
    NetSession& operator=(const NetSession &) = delete;
private:
    bool SendNetPkg(const NetPackage &pkg);
    void Update();
public:
    void OnReadCallback();
    void OnWriteCallback();
    void OnWriteCompleteCallback();
    void OnErrorCallback(MError err);
private:
    MNetConnector connector_;
    MNetEventLoopThread *p_event_loop_thread_;
    uint16_t recv_pkg_len_;
    bool recv_pkg_len_read_;
    std::mutex recv_pkg_mtx_;
    std::list<NetPackage> recv_pkg_list_;
    std::mutex send_pkg_mtx_;
    std::list<NetPackage> send_pkg_list_;
};

#endif
