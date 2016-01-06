#include <net/m_net_connector.h>
#include <net/m_net_listener.h>
#include <net/m_net_event_handler.h>
#include <cstring>

MNetConnector::MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventHandler *p_event_handler
        , std::function<void ()> read_cb, std::function<void ()> write_complete_cb, std::function<void (MNetError)> error_cb
        , bool need_free_sock, size_t read_len, size_t write_len)
    :p_sock_(p_sock)
    ,event_(p_sock, p_event_handler, nullptr, nullptr, nullptr)
    ,p_listener_(p_listener)
    ,p_event_handler_(p_event_handler)
    ,read_cb_(read_cb)
    ,write_complete_cb_(write_complete_cb)
    ,error_cb_(error_cb)
    ,need_free_sock_(need_free_sock)
{
    event_.SetReadCallback(std::bind(&MNetConnector::OnReadCallback, this));
    event_.SetWriteCallback(std::bind(&MNetConnector::OnWriteCallback, this));
    event_.SetErrorCallback(std::bind(&MNetConnector::OnErrorCallback, this, std::placeholders::_1));
}

MNetConnector::~MNetConnector()
{
    if (need_free_sock_ && p_sock_)
    {
        delete p_sock_;
    }
}

void MNetConnector::SetSocket(MSocket *p_sock)
{
    p_sock_ = p_sock;
}

MSocket* MNetConnector::GetSocket()
{
    return p_sock_;
}

MNetEvent& MNetConnector::GetEvent()
{
    return event_;
}

void MNetListener::SetListener(MNetListener *p_listener)
{
    p_listener_ = p_listener;
}

MNetListener* MNetConnector::GetListener()
{
    return p_listener_;
}

void MNetConnector::SetEventHandler(MNetEventHandler *p_event_handler)
{
    p_event_handler_ = p_event_handler;
}

MNetEventHandler* MNetConnector::GetEventHandler()
{
    return p_event_handler_;
}

void MNetConnector::SetReadCallback(std::function<void ()> read_cb)
{
    read_cb_ = read_cb;
}

std::function<void ()>& MNetConnector::GetReadCallback()
{
    return read_cb_;
}

void MNetConnector::SetWriteCompleteCallback(std::function<void ()> write_complete_cb)
{
    write_complete_cb_ = write_complete_cb;
}

std::function<void ()>& MNetConnector::GetWriteCompleteCallback()
{
    return write_complete_cb_;
}

void MNetConnector::SetErrorCallback(std::function<void (MNetError)> error_cb)
{
    error_cb_ = error_cb;
}

std::function<void (MNetError)>& MNetConnector::GetErrorCallback()
{
    return error_cb_;
}

void MNetConnector::SetNeedFreeSock(bool need)
{
    need_free_sock_ = need;
}

bool MNetConnector::GetNeedFreeSock() const
{
    return need_free_sock_;
}

MNetError MNetConnector::EnableReadWrite(bool enable)
{
    if (enable)
    {
        return event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_EDGE);
    }
    else
    {
        return event_.DisableEvents();
    }
}

MNetError MNetConnector::ReadBuf(void *p_buf, size_t len)
{
    std::lock_guard<std::mutex> lock(read_mtx_);
    if (p_read_buf_end_ >= p_read_buf_start_)
    {
        if (p_read_buf_end_ - p_read_buf_start_ < len)
        {
            return MNetError::BufferInsufficient;
        }
        memcpy(p_buf, p_read_buf_start_, len);
        p_read_buf_start_ += len;
        return MNetError::No;
    }
    else
    {
        if (len <= (read_buf_len_ - (p_read_buf_start_ - p_read_buf_)))
        {
            memcpy(p_buf, p_read_buf_start_, len);
            p_read_buf_start_ += len;
            return MNetError::No;
        }
        else if (len <= (read_buf_len_ - (p_read_buf_start_ - p_read_buf_end_)))
        {
            size_t end_len = read_buf_len_ - (p_read_buf_start_ - p_read_buf_);
            memcpy(p_buf, p_read_buf_start_, end_len);
            p_read_buf_start_ += (read_buf_len_ - len);
            memcpy(p_buf + end_len, p_read_buf_, p_read_buf_start_ - p_read_buf_);
            return MNetError::No;
        }
        else
        {
            return MNetError::BufferInsufficient;
        }
    }
}

MNetError MNetConnector::WriteBuf(const void *p_buf, size_t len)
{
    if (write_ready_)
    {
        std::pair<int, MNetError> ret = p_sock_->Send(p_buf, len);

    }
    else
    {
        std::lock_guard<std::mutex> lock(write_mtx_);
    }
}

void MNetConnector::OnReadCallback()
{
}

void MNetConnector::OnWriteCallback()
{
}

void MNetConnector::OnErrorCallback(MNetError err)
{
}
