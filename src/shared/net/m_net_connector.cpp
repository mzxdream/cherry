#include <net/m_net_connector.h>
#include <net/m_socket.h>
#include <net/m_net_listener.h>
#include <net/m_net_event_loop.h>

MNetConnector::MNetConnector(MSocket *p_sock, MNetListener *p_listener, MNetEventLoop *p_event_loop
        , const std::function<void ()> &read_cb, const std::function<void ()> &write_complete_cb, const std::function<void (MNetError)> &error_cb
        , bool need_free_sock, size_t read_len, size_t write_len)
    :p_sock_(p_sock)
    ,event_(p_sock ? p_sock->GetHandler() : -1, p_event_loop, nullptr, nullptr, nullptr)
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
    event_.SetFD(p_sock_ ? p_sock_->GetHandler() : -1);
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
    event_.SetEventLoop(p_event_loop_);
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
        event_.SetReadCallback(std::bind(&MNetConnector::OnReadCallback, this));
        event_.SetWriteCallback(std::bind(&MNetConnector::OnWriteCallback, this));
        event_.SetErrorCallback(std::bind(&MNetConnector::OnErrorCallback, this, std::placeholders::_1));
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
            if (static_cast<size_t>(ret.first) < len)
            {
                if (!write_buffer_.Append(p_buf + ret.first, len - ret.first))
                {
                    return MNetError::BufferOverflow;
                }
                write_ready_ = false;
                return event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_WRITE|M_NET_EVENT_EDGE);
            }
            if (write_complete_cb_)
            {
                write_complete_cb_();
            }
            return MNetError::No;
        }
        else if (ret.second == MNetError::InterruptedSysCall
            || ret.second == MNetError::Again)
        {
            write_ready_ = false;
            return event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_WRITE|M_NET_EVENT_EDGE);
        }
        else
        {
            return MNetError::Unknown;
        }
    }
    else
    {
        return write_buffer_.Append(p_buf, len) ? MNetError::No : MNetError::BufferOverflow;
    }
}

void MNetConnector::OnReadCallback()
{
    std::pair<char*, size_t> buf;
    std::pair<int, MNetError> ret;
    while (true)
    {
        buf = read_buffer_.GetNextCapacity();
        if (!buf.first || buf.second == 0)
        {
            OnErrorCallback(MNetError::BufferOverflow);
            return;
        }
        ret = p_sock_->Recv(buf.first, static_cast<int>(buf.second));
        if (ret.second == MNetError::No)
        {
            if (ret.first == 0)
            {
                OnErrorCallback(MNetError::Disconnect);
                return;
            }
            else
            {
                if (!read_buffer_.AddEndLen(ret.first))
                {
                    OnErrorCallback(MNetError::Unknown);
                    return;
                }
                if (static_cast<size_t>(ret.first) < buf.second)
                {
                    if (read_cb_)
                    {
                        read_cb_();
                    }
                    return;
                }
            }
        }
        else if (ret.second == MNetError::InterruptedSysCall
            || ret.second == MNetError::Again)
        {
            if (read_cb_)
            {
                read_cb_();
            }
            return;
        }
        else
        {
            OnErrorCallback(ret.second);
            return;
        }
    }
}

void MNetConnector::OnWriteCallback()
{
    std::pair<const char*, size_t> buf;
    std::pair<int, MNetError> ret;
    while (true)
    {
        buf = read_buffer_.GetNextData();
        if (!buf.first || buf.second == 0)
        {
            MNetError err = event_.EnableEvents(M_NET_EVENT_READ|M_NET_EVENT_EDGE);
            if (err != MNetError::No)
            {
                OnErrorCallback(err);
                return;
            }
            write_ready_ = true;
            if (write_complete_cb_)
            {
                write_complete_cb_();
            }
            return;
        }
        ret = p_sock_->Send(buf.first, static_cast<int>(buf.second));
        if (ret.second == MNetError::No)
        {
            if (!write_buffer_.AddStartLen(ret.first))
            {
                OnErrorCallback(MNetError::Unknown);
                return;
            }
            if (static_cast<size_t>(ret.first) < buf.second)
            {
                return;
            }
        }
        else if (ret.second == MNetError::InterruptedSysCall
            || ret.second == MNetError::Again)
        {
            return;
        }
        else
        {
            OnErrorCallback(ret.second);
            return;
        }
    }
}

void MNetConnector::OnErrorCallback(MNetError err)
{
    event_.DisableEvents();
    if (error_cb_)
    {
        error_cb_(err);
    }
}
