#include <net/net_manager.h>
#include <lock_guard>
#include <iostream>

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
        work = new NetThread();
        if (!p_work || !p_work.Init())
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
        delete work;
    }
    for (auto &listener : listener_list_)
    {
        delete listener;
    }
    for (auto &session : session_list_)
    {
        if (session && session->p_connector)
        {
            delete session->p_connector;
        }
        delete session;
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

    p_listener->SetAcceptCallback(std::bind(&NetManager::OnConnectCallback, this, p_listener, std::placeholders::_1));
    p_listener->SetErrorCallback(std::bind(&NetManager::OnListenerErrorCallback, this, listener_list_.size()-1, std::placeholders::_1));

    MNetError err = p_listener->EnableAccept(true);
    if (err != MNetError::No)
    {
        return false;
    }
    return true;
}

void OnCloseSessionCallback(NetSession *p_session)
{
    if (p_session)
    {
        if (p_session->p_connector)
        {
            delete p_session->p_connector;
        }
        delete p_session;
    }
}

void NetManager::CloseSession(NetSession *p_session)
{
    if (!p_session)
    {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(session_mutex_);
        auto it = session_list_.find(p_session);
        if (it != session_list_.end())
        {
            session_list_.erase(it);
        }
    }
    if (p_session->p_loop_thread)
    {
        p_session->p_loop_thread->AddCallback(std::bind(OnCloseSessionCallback, p_session));
    }
    else
    {
        delete p_session;
    }
}

void OnWriteSessionCallback(NetSession *p_session, char *p_buf, size_t len)
{
    p_session->p_connector->WriteBuf(p_buf, len);
    delete p_buf;
}

void NetManager::WriteSession(NetSession *p_session, char *p_buf, size_t len)
{
    if (!p_session)
    {
        return;
    }
    p_session->p_loop_thread->AddCallback(std::bind(OnWriteSessionCallback, p_session, p_buf, len));
}

void NetManager::OnReadCallback(NetSession *p_session)
{
    char tmp[10];
    if (p_session->p_connector->ReadBuf(tmp, 10) == MNetError::No)
    {
        std::cout << p_session->p_connector->GetListener()->GetSocket()->GetIP() << " "
            << p_session->p_connector->GetListener()->GetSocket()->GetPort() << " "
            << tmp << std::endl;
    }
}

void NetManager::OnCloseCallback(NetSession *p_session, MNetError err)
{
    std::cout << p_session->p_connector->GetListener()->GetSocket()->GetIP() << " "
        << p_session->p_connector->GetListener()->GetSocket()->GetPort() << " "
        << " has socket disconnect:" << static_cast<int>(err) << std::endl;
    std::lock_guard<std::mutex> lock(session_mutex_);
    auto it = session_list_.find(p_session);
    if (it != session_list_.end())
    {
        session_list_.erase(it);
    }
    delete p_session->p_connector;
    delete p_session;
}

void NetManager::OnConnectCallback(MNetListener *p_listener, MSocket *p_sock)
{
    if (!p_sock)
    {
        return;
    }
    NetThread *p_loop_thread = GetMinEventsThread();
    if (!p_loop_thread)
    {
        delete p_sock;
        return;
    }
    MNetConnector *p_connector = new MNetConnector(p_sock, p_listener, &(p_loop_thread->GetEventLoop()), nullptr, nullptr, nullptr, true, 1024, 1024);
    if (!p_connector)
    {
        delete p_sock;
        return;
    }
    NetSession *p_session = new NetSession();
    p_session->p_connector = p_connector;
    p_session->p_loop_thread = p_loop_thread;

    std::lock_guard<std::mutex> lock(session_mutex_);
    p_connector->SetReadCallback(std::bind(OnReadCallback, p_session));
    p_connector->SetErrorCallback(std::bind(OnCloseCallback, p_session));
    p_connector->EnableReadWrite(true);
}

void NetManager::OnListenerErrorCallback(size_t pos, MNetError err)
{
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

NetThread* NetManager::GetMinEventsThread()
{
    if (work_list_.empty())
    {
        return nullptr;
    }
    NetThread *p_loop_thread = work_list_[0];
    size_t count = work_list_[0]->GetEventLoop().GetEventCount();
    for (size_t i = 1; i < work_list_.size(); ++i)
    {
        if (count > work_list_[i]->GetEventLoop().GetEventCount())
        {
            count = work_list_[i]->GetEventLoop().GetEventCount();
            p_event_loop = work_list_[i];
        }
    }
    return p_loop_thread;
}
