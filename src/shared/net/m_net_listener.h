#ifndef _M_NET_LISTENER_H_
#define _M_NET_LISTENER_H_

#include <net/m_net_event.h>

class MSocket;
class MNetEventHandler;

class MNetListener
{
public:
    explicit MNetListener(MSocket &sock, MNetEventHandler &event_handler, std::function<void ()> accept_cb);
    ~MNetListener();
    MNetListener(const MNetListener &) = delete;
    MNetListener& operator=(const MNetListener &) = delete;
public:
    MNetError EnableAccept(bool enable);
private:
    MSocket &sock_;
    MNetEvent event_;
    MNetEventHandler &event_handler_;
    std::function<void ()> accept_cb_;
};

#endif
