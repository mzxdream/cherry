#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <net/m_socket.h>
#include <net/m_net_event_loop.h>
#include <net/m_net_connector.h>
#include <net/m_net_listener.h>
#include <thread/m_thread.h>
#include <mutex>
#include <functional>
#include <list>
#include <util/m_singleton.h>
#include <set>

class NetThread
    :public MThread
{
public:
    NetThread();
    virtual ~NetThread();
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

struct NetSession
{
    MNetConnector *p_connector;
    NetThread *p_loop_thread;
};

class NetManager
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
    void CloseSession(NetSession *p_session);
    void WriteSession(NetSession *p_session, char *p_buf, size_t len);
    void WriteAll(const char *p_buf, size_t len);
public:
    void OnConnectCallback(MNetListener *p_listener, MSocket *p_sock);
    void OnListenerErrorCallback(size_t pos, MNetError err);
    void OnReadCallback(NetSession *p_session);
    void OnCloseCallback(NetSession *p_session, MNetError err);
private:
    NetThread* GetMinEventsThread();
private:
    std::vector<NetThread*> work_list_;
    std::vector<MNetListener*> listener_list_;
    std::mutex session_mutex_;
    std::set<NetSession*> session_list_;
};

#endif
