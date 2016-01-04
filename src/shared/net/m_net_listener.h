#ifndef _M_NET_LISTENER_H_
#define _M_NET_LISTENER_H_

#include <net/m_net_event.h>

class MSocket;
class MNetEventHandler;

class MNetListener
{
public:
    explicit MNetListener(MSocket *p_sock, MNetEventHandler *p_event_handler);
    ~MNetListener();
    MNetListener(const MNetListener &) = delete;
    MNetListener& operator=(const MNetListener &) = delete;
public:
    void SetSocket(MSocket *p_sock);
    MSocket* GetSocket();
    MNetEvent& GetEvent();
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler* GetEventHandler();

    MNetError EnableAccept();
    MNetError AsyncAccept(MSocket *p_sock, std::function<void (MNetError)> accept_cb);
public:
    void OnAccecptCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket *p_sock_;
    MNetEvent event_;
    MNetEventHandler *p_event_handler_;
    MSocket *p_accept_sock_;
    std::function<void (MNetError)> accept_cb_;
    bool accept_actived_;
};

#endif
