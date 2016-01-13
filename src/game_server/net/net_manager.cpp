#include <net/net_manager.h>
#include <lock_guard>

NetThread::NetThread()
{
}

NetThread::~NetThread()
{
}

bool NetThread::Init()
{
    return event_loop_.Create() == MNetError::No;
}

void NetThread::Close()
{
    StopAndJoin();
    event_loop_.Close();
}

MNetEventLoop& NetThread::GetEventLoop()
{
    return event_loop_;
}

void NetThread::AddCallback(const std::function<void ()> &callback)
{
    {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        callback_list_.push_back(callback);
    }
    event_loop_.Interrupt();
}

void NetThread::DoRun()
{
    event_loop_.ProcessEvents();
    std::list<std::function<void ()> > cb_list;
    {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        cb_list.swap(callback_list_);
    }
    for (auto &cb : cb_list)
    {
        if (cb)
        {
            cb();
        }
    }
}

NetManager::NetManager()
{
}

NetManager::~NetManager()
{
    Close();
}

bool NetManager::Init(size_t work_count)
{
    work_list_.resize(work_count);
    for (auto &work : work_list_)
    {
        if (!work.Init())
        {
            return false;
        }
    }
    return true;
}

void NetManager::Close()
{
    for (auto &work : work_list_)
    {
        work.Close();
    }
    for (auto &listener : listener_list_)
    {
        delete listener;
    }
}

bool NetManager::AddListener(const std::string &ip, unsigned short port)
{
    MSocket *p_sock = new MSocket();
    if (!p_sock)
    {
        return false;
    }
    if (p_sock->CreateNonblockReuseListener(ip, port, 128) != MNetError::No)
    {
        delete p_sock;
        return false;
    }
    MNetListener *p_listener = new MNetListener(p_sock, GetMinEventsEventLoop(), nullptr, nullptr, true, 5);
    if (!p_listener)
    {
        return false;
    }
    listener_list_.push_back(p_listener);

    p_listener->SetAcceptCallback(std::bind(&NetManager::OnConnectCallback, this, std::placeholders::_1));
    p_listener->SetErrorCallback(std::bind(&NetManager::OnListenerErrorCallback, this, listener_list_.size()-1, std::placeholders::_1));

    MNetError err = p_listener->EnableAccept(true);
    if (err != MNetError::No)
    {
        return false;
    }
    return true;
}

void NetManager::CloseConnect(MNetConnector *p_connector)
{
    if (!p_connector)
    {
        return;
    }
    if (!p_connector->GetEventLoop())
    {
        return;
    }
}

void NetManager::OnConnectCallback(MSocket *p_sock)
{

}

void NetManager::OnListenerErrorCallback(size_t pos, MNetError err)
{
    //not thread safe
    if (listener_list_.size() >= pos)
    {
        return;
    }
    MNetListener *p_listener = listener_list_[pos];
    if (!p_listener)
    {
        return;
    }
    std::cout << "listener ip:" << p_listener->GetSocket()->GetIP()
        << " port:" << p_listener->GetSocket()->GetPort() << std::endl;
}

MNetEventLoop* NetManager::GetMinEventsEventLoop()
{
    if (work_list_.empty())
    {
        return nullptr;
    }
    MNetEventLoop *p_event_loop = &(work_list_[0].GetEventLoop());
    size_t count = work_list_[0].GetEventLoop().GetEventCount();
    for (size_t i = 1; i < work_list_.size(); ++i)
    {
        if (count > work_list_[i].GetEventLoop().GetEventCount())
        {
            count = work_list_[i].GetEventLoop().GetEventCount();
            p_event_loop = &(work_list_[0].GetEventLoop());
        }
    }
    return p_event_loop;
}
