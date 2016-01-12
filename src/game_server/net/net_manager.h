#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <net/net_thread.h>

class NetManager
{
public:
    NetManager();
    ~NetManager();
    NetManager(const NetManager &) = delete;
    NetManager& operator=(const NetManager &) = delete;
public:
    bool Init(size_t handler_count);
    void Close();

    MNetError AddListener(const std::string &ip, unsigned short port);
public:
    void CloseConnect(MNetConnector*);
    void OnConnectCallback(MSocket *p_sock);
    void OnErrorCallback();
private:
    std::vector<NetThread*> thread_list_;
    std::vector<MNetListener*> listener_list_;
    std::set<MNetConnector*> connector_list_;
};

#endif
