#include <net/m_net_event.h>

MNetEvent::MNetEvent()
    :event_(0)
{
}

MNetEvent::~MNetEvent()
{
}

void MNetEvent::SetEvent(int event)
{
    event_ = event;
}

int MNetEvent::GetEvent() const
{
    return event_;
}

void MNetEvent::SetReadCallback(const std::function<void ()> &cb_read)
{
    cb_read_ = cb_read;
}

void MNetEvent::SetWriteCallback(const std::function<void ()> &cb_write)
{
    cb_write_ = cb_write;
}

void MNetEvent::SetCloseCallback(const std::function<void ()> &cb_close)
{
    cb_close_ = cb_close;
}

void MNetEvent::OnRead()
{
    if (cb_read_)
    {
        cb_read_();
    }
}

void MNetEvent::OnWrite()
{
    if (cb_write_)
    {
        cb_write_();
    }
}

void MNetEvent::OnClose()
{
    if (cb_close_)
    {
        cb_close_();
    }
}
