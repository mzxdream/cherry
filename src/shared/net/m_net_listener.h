#ifndef _M_NET_LISTENER_H_
#define _M_NET_LISTENER_H_

#include <net/m_net_event.h>

class MSocket;
class MNetEventHandler;

class MNetListener
{
public:
    MNetListener(MSocket &sock, MNetEventHandler &event_handler);
    ~MNetListener();
    MNetListener(const MNetListener &) = delete;
    MNetListener& operator=(const MNetListener &) = delete;
public:
    MSocket& GetSocket();
    MNetEvent& GetEvent();
    MNetEventHandler& GetEventHandler();

private:
    MSocket &sock_;
    MNetEvent event_;
    MNetEventHandler &event_handler_;
};

#endif
