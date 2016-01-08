#include <net/m_net_connector.h>
#include <net/m_socket.h>
#include <net/m_net_listener.h>
#include <net/m_net_event_loop.h>
#include <cstring>

MNetConnector::MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventLoop *p_event_loop
        , const std::function<void ()> &read_cb, const std::function<void ()> &write_complete_cb, const std::function<void (MNetError)> &error_cb
        , bool need_free_sock, size_t read_len, size_t write_len)
    :p_sock_(p_sock)
    ,event_(p_sock->GetHandler(), p_event_loop, nullptr, nullptr, nullptr)
    ,p_listener_(p_listener)
    ,p_event_loop_(p_event_loop)
    ,read_cb_(read_cb)
    ,write_complete_cb_(write_complete_cb)
    ,error_cb_(error_cb)
    ,need_free_sock_(need_free_sock)
    ,read_buffer_(read_len)
    ,write_buffer_(write_len)
    ,write_ready_(true)
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

void MNetConnector::SetListener(MNetListener *p_listener)
{
    p_listener_ = p_listener;
}

MNetListener* MNetConnector::GetListener()
{
    return p_listener_;
}

void MNetConnector::SetEventLoop(MNetEventLoop *p_event_loop)
{
    p_event_loop_ = p_event_loop;
}

MNetEventLoop* MNetConnector::GetEventLoop()
{
    return p_event_loop_;
}

void MNetConnector::SetReadCallback(const std::function<void ()> &read_cb)
{
    read_cb_ = read_cb;
}

std::function<void ()>& MNetConnector::GetReadCallback()
{
    return read_cb_;
}

void MNetConnector::SetWriteCompleteCallback(const std::function<void ()> &write_complete_cb)
{
    write_complete_cb_ = write_complete_cb;
}

std::function<void ()>& MNetConnector::GetWriteCompleteCallback()
{
    return write_complete_cb_;
}

void MNetConnector::SetErrorCallback(const std::function<void (MNetError)> &error_cb)
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
    if (!read_buffer_.Peek(p_buf, len))
    {
        return MNetError::BufferInsufficient;
    }
    return MNetError::No;
}

MNetError MNetConnector::WriteBuf(const char *p_buf, size_t len)
{
    if (write_ready_)
    {
        std::pair<int, MNetError> ret = p_sock_->Send(p_buf, len);
        if (ret.second == MNetError::No)
        {
            if (ret.first < len)
            {
                write_buffer_.Append(p_buf + ret.first, len - ret.first);
                write_ready_ = false;
                return event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_WRITE|M_NET_EVENT_EDGE);
            }
            return MNetError::No;
        }
        else if (ret.second == )
        {

        }
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
