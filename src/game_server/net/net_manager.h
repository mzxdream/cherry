#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <net/m_net_event_loop.h>
#include <thread/m_thread.h>
#include <mutex>
#include <functional>
#include <list>
#include <util/m_singleton.h>

class NetThread
    :public MThread
{
public:
    NetThread();
    ~NetThread();
    NetThread(const NetThread &) = delete;
    NetThread& operator=(const NetThread &) = delete;
public:
    bool Init();
    void Close();
    MNetEventLoop& GetEventLoop();
    void AddCallback(const std::function<void ()> &callback);
private:
    virtual void DoRun();
private:
    MNetEventLoop event_loop_;
    std::list<std::function<void ()> > callback_list_;
    std::mutex callback_mutex_;
};


class NetManager
    :public MSingleton<NetManager, std::mutex>
{
public:
    NetManager();
    ~NetManager();
    NetManager(const NetManager &) = delete;
    NetManager& operator=(const NetManager &) = delete;
public:
    bool Init(size_t work_count);
    void Close();

    bool AddListener(const std::string &ip, unsigned short port);
    void CloseConnect(MNetConnector *p_connector);
public:
    void OnConnectCallback(MSocket *p_sock);
    void OnListenerErrorCallback(size_t pos, MNetError err);
private:
    bool GetMinEventsWork();
private:
    std::vector<NetThread> work_list_;
    std::vector<MNetListener*> listener_list_;
    std::set<MNetConnector*> connector_list_;
};

#endif
