#include <net/m_net_event.h>
#include <net/m_socket.h>
#include <net/m_net_event_handler.h>

MNetEvent::MNetEvent(MSocket &sock, MNetEventHandler &event_handler)
    :sock_(sock)
    ,event_handler_(event_handler)
    ,enable_read_(false)
    ,enable_write_(false)
{
}

MNetEvent::~MNetEvent()
{
    EnableRead(false);
    EnableWrite(false);
}

MNetError MNetEvent::EnableRead(bool enable)
{
    if (enable == enable_read_)
    {
        return MNetError::No;
    }
    if (enable)
    {
        if (enable_write_)
        {
            int events = M_NET_EVENT_READ | M_NET_EVENT_WRITE | M_NET_EVENT_EDGE;
            MNetError err = event_handler_.ModEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
        else
        {
            int events = M_NET_EVENT_READ | M_NET_EVENT_EDGE;
            MNetError err = event_handler_.AddEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
    }
    else
    {
        if (enable_write_)
        {
            int events = M_NET_EVENT_WRITE;
            MNetError err = event_handler_.ModEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
        else
        {
            MNetError err = event_handler_.DelEvent(sock_.GetHandler());
            if (err != MNetError::No)
            {
                return err;
            }
        }
    }
    enable_read_ = enable;
}

MNetError MNetEvent::EnableWrite(bool enable)
{
    if (enable == enable_write_)
    {
        return MNetError::No;
    }
    if (enable)
    {
        if (enable_read_)
        {
            int events = M_NET_EVENT_READ | M_NET_EVENT_WRITE | M_NET_EVENT_EDGE;
            MNetError err = event_handler_.ModEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
        else
        {
            int events = M_NET_EVENT_WRITE;
            MNetError err = event_handler_.AddEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
    }
    else
    {
        if (enable_read_)
        {
            int events = M_NET_EVENT_READ | M_NET_EVENT_EDGE;
            MNetError err = event_handler_.ModEvent(sock_.GetHandler(), events, this);
            if (err != MNetError::No)
            {
                return err;
            }
        }
        else
        {
            MNetError err = event_handler_.DelEvent(sock_.GetHandler());
            if (err != MNetError::No)
            {
                return err;
            }
        }
    }
    enable_write_ = enable;
}

void MNetEvent::SetReadCallback(std::function<void ()> read_cb)
{
    read_cb_ = read_cb;
}

void MNetEvent::SetWriteCallback(std::function<void ()> write_cb)
{
    write_cb_ = write_cb;
}

void MNetEvent::SetErrorCallback(std::function<void (MNetError)> error_cb)
{
    error_cb_ = error_cb;
}

void MNetEvent::OnReadCallback()
{
    if (read_cb_)
    {
        read_cb_();
    }
}

void MNetEvent::OnWriteCallback()
{
    if (write_cb_)
    {
        write_cb_();
    }
}

void MNetEvent::OnErrorCallback(MNetError err)
{
    if (error_cb_)
    {
        error_cb_(err);
    }
}
