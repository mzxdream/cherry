#ifndef _M_NET_LISTENER_H_
#define _M_NET_LISTENER_H_

#include <net/m_net_event.h>

class MSocket;
class MNetEventHandler;

class MNetListener
{
public:
    explicit MNetListener(MSocket *p_sock, MNetEventHandler *p_event_handler
        , std::function<void (MSocket*)> accept_cb, std::function<void (MNetError)> error_cb, bool need_free_sock);
    ~MNetListener();
    MNetListener(const MNetListener &) = delete;
    MNetListener& operator=(const MNetListener &) = delete;
public:
    void SetSocket(MSocket *p_sock);
    MSocket* GetSocket();
    MNetEvent& GetEvent();
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler* GetEventHandler();
    void SetAcceptCallback(std::function<void (MSocket*)> accept_cb);
    std::function<void (MSocket*)>& GetAcceptCallback();
    void SetErrorCallback(std::function<void (MNetError)> error_cb);
    std::function<void (MNetError)>& GetErrorCallback();
    void SetNeedFreeSock(bool need);
    bool GetNeedFreeSock() const;

    MNetError EnableAccept(bool enable);
public:
    void OnAcceptCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket *p_sock_;
    MNetEvent event_;
    MNetEventHandler *p_event_handler_;
    std::function<void (MSocket*)> accept_cb_;
    std::function<void (MNetError)> error_cb_;
    bool need_free_sock_;
};

#endif
