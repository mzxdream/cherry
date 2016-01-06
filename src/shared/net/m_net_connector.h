#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_net_event.h>
#include <mutex>

class MSocket;
class MNetListener;
class MNetEventHandler;

class MNetConnector
{
public:
    MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventHandler *p_event_handler
        , std::function<void ()> read_cb, std::function<void ()> write_complete_cb, std::function<void (MNetError)> error_cb
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
    void SetEventHandler(MNetEventHandler *p_event_handler);
    MNetEventHandler* GetEventHandler();
    void SetReadCallback(std::function<void ()> read_cb);
    std::function<void ()>& GetReadCallback();
    void SetWriteCompleteCallback(std::function<void ()> write_complete_cb);
    std::function<void ()>& GetWriteCompleteCallback();
    void SetErrorCallback(std::function<void (MNetError)> error_cb);
    std::function<void (MNetError)>& GetErrorCallback();
    void SetNeedFreeSock(bool need);
    bool GetNeedFreeSock() const;

    MNetError EnableReadWrite(bool enable);

    MNetError ReadBuf(void *p_buf, size_t len);
    MNetError WriteBuf(const void *p_buf, size_t len);
public:
    void OnReadCallback();
    void OnWriteCallback();
    void OnErrorCallback(MNetError err);
private:
    MSocket *p_sock_;
    MNetEvent event_;
    MNetListener *p_listener_;
    MNetEventHandler *p_event_handler_;
    std::function<void ()> read_cb_;
    std::function<void ()> write_complete_cb_;
    std::function<void (MNetError)> error_cb_;
    bool need_free_sock_;
    char *p_read_buf_;
    size_t read_buf_len_;
    char *p_read_buf_start_;
    char *p_read_buf_end_;
    std::mutex read_mtx_;
    char *p_write_buf_;
    size_t write_buf_len_;
    char *p_write_buf_start_;
    char *p_write_buf_end_;
    bool write_ready_;
    std::mutex write_mtx_;
};

#endif


