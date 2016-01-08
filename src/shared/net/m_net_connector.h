#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_net_event.h>
#include <util/m_circle_buffer.h>

class MSocket;
class MNetListener;
class MNetEventLoop;

class MNetConnector
{
public:
    explicit MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventLoop *p_event_loop
        , const std::function<void ()> &read_cb, const std::function<void ()> &write_complete_cb, const std::function<void (MNetError)> &error_cb
        , bool need_free_sock, size_t read_len, size_t write_len);
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
    void SetSocket(MSocket *p_sock);
    MSocket* GetSocket();
    MNetEvent& GetEvent();
    void SetListener(MNetListener *p_listener);
    MNetListener* GetListener();
    void SetEventLoop(MNetEventLoop *p_event_loop);
    MNetEventLoop* GetEventLoop();
    void SetReadCallback(const std::function<void ()> &read_cb);
    std::function<void ()>& GetReadCallback();
    void SetWriteCompleteCallback(const std::function<void ()> &write_complete_cb);
    std::function<void ()>& GetWriteCompleteCallback();
    void SetErrorCallback(const std::function<void (MNetError)> &error_cb);
    std::function<void (MNetError)>& GetErrorCallback();
    void SetNeedFreeSock(bool need);
    bool GetNeedFreeSock() const;

    MNetError EnableReadWrite(bool enable);

    MNetError ReadBuf(void *p_buf, size_t len);
    MNetError WriteBuf(const char *p_buf, size_t len);
public:
    void OnReadCallback();
    void OnWriteCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket *p_sock_;
    MNetEvent event_;
    MNetListener *p_listener_;
    MNetEventLoop *p_event_loop_;
    std::function<void ()> read_cb_;
    std::function<void ()> write_complete_cb_;
    std::function<void (MNetError)> error_cb_;
    bool need_free_sock_;
    MCircleBuffer read_buffer_;
    MCircleBuffer write_buffer_;
    bool write_ready_;
};

#endif


