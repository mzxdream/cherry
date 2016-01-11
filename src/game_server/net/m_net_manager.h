#ifndef _M_NET_MANAGER_H_
#define _M_NET_MANAGER_H_

#include <net/m_socket.h>
#include <net/m_net_listener.h>
#include <net/m_net_connector.h>
#include <net/m_net_event.h>
#include <net/m_net_event_handler.h>

class MNetManager
{
public:
    explicit MNetManager(size_t handler_count);
    ~MNetManager();
    MNetManager(const MNetManager &) = delete;
    MNetManager& operator=(const MNetManager &) = delete;
public:
    MNetError Init();
    MNetError Close();
    size_t GetHandlerCount() const;
    MNetEventHandler* GetEventHandler(size_t index);
    MNetEventHandler* GetMinEventsEventHandler();

    MNetError AddListener(const std::string &ip, unsigned short port, int backlog, std::function<void (MNetConnector*)> accept_cb);
    void OnAcceptCallback(size_t index);
private:
    std::vector<MNetEventHandler*> handler_list_;
    std::vector<MSocket*> listener_sock_list_;
    std::vector<std::pair<MNetListener*, std::function<void (MNetConnector*)> > > listener_event_list_;
};

#endif
