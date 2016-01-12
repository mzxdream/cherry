#include <net/net_thread.h>
#include <lock_guard>

NetThread::NetThread()
{
    event_loop_.Create();
}

NetThread::~NetThread()
{
}

MNetEventLoop& NetThread::GetEventLoop()
{
    return event_loop_;
}

void NetThread::AddCallback(const std::function<void ()> &callback)
{
    {
        std::lock_guard<std::mutex> lock;
        callback_list_.push_back(callback);
    }
    event_loop_.Interrupt();
}

void NetThread::DoRun()
{
    ProcessEvents();
    std::list<std::function<void ()> > cb_list;
    {
        std::lock_guard<std::mutex> lock;
        cb_list.swap(callback_list_);
    }
    for (auto &it : cb_list)
    {
        if (it)
        {
            it();
        }
    }
}
