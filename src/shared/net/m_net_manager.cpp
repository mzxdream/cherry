#include <net/m_net_manager.h>

MNetManager::MNetManager(size_t handler_count)
    :handler_list_(handler_count, nullptr)
{
}

MNetManager::~MNetManager()
{
    Close();
}

MNetError MNetManager::Init()
{
    for (size_t i = 0; i < handler_list_.size(); ++i)
    {
        handler_list_[i] = new MNetEventHandler();
        if (handler_list_[i] == nullptr)
        {
            return MNetError::OutOfMemory;
        }
        MNetError err = handler_list_[i].Create();
        if (err != MNetError::No)
        {
            return err;
        }
        if (handler_list_[i].Start() != MThreadError::No)
        {
            return MNetError::Unknown;
        }
    }
    return MNetError::No;
}

MNetError MNetManager::Close()
{
    for ()
    for (size_t i = 0; i < handler_list_.size(); ++i)
    {
        if (handler_list_[i])
        {
            handler_list_[i].StopAndJoin();
        }
    }
}

size_t MNetManager::GetHandlerCount() const
{
    return handler_list_.size();
}

MNetEventHandler* MNetManager::GetEventHandler(size_t index)
{
    if (index >= handler_list_.size())
    {
        return nullptr;
    }
    return handler_list_[index];
}

MNetEventHandler* MNetManager::GetMinEventsEventHandler()
{
    if (handler_list_.empty())
    {
        return nullptr;
    }
    MNetEventHandler *p_handler = handler_list_[0];
    size_t count = handler_list_[0]->GetEventCount();
    for (size_t i = 1; i < handler_list_.size(); ++i)
    {
        if (count > handler_list_[i].GetEventCount())
        {
            p_handler = handler_list_[i];
            count = handler_list_[i]->GetEventCount();
        }
    }
    return p_handler;
}

MNetError MNetManager::AddListener(const std::string &ip, unsigned short port, int backlog, std::function<void (MNetConnector*)> accept_cb)
{

    MSocket *p_sock = new MSocket();
    if (!p_sock)
    {
        return MNetError::OutOfMemory;
    }
    MNetError err = MNetError::No;
    do
    {
        err = p_sock->Create(MSocketFamily::IPV4, MSocketType::TCP, MSocketProtocol::Default);
        if (err != MNetError::No)
        {
            break;
        }
        err = p_sock->SetBlock(false);
        if (err != MNetError::No)
        {
            break;
        }
        err = p_sock->SetReUseAddr(true);
        if (err != MNetError::No)
        {
            break;
        }
        err = p_sock->Bind(ip, port);
        if (err != MNetError::No)
        {
            break;
        }
        err = p_sock->Listen(backlog);
        if (err != MNetError::No)
        {
            break;
        }

        MNetEventHandler *p_handler = GetMinEventsEventHandler();
        if (!p_handler)
        {
            err = MNetError::EventHandlerInvalid;
            break;
        }
        MNetListener *p_listener = new MNetListener(*p_sock, *p_handler, std::bind(&MNetManager::OnAcceptCallback, this, listener_list_.size()));
        if (!p_listener)
        {
            err = MNetError::OutOfMemory;
            break;
        }
        err = p_listener->EnableAccept();
        if (err != MNetError::No)
        {
            delete p_listener;
            break;
        }
        listener_event_list_.push_back(std::pair<MNetListener*, std::function<void (MNetConnector*)> >(p_listener, accept_cb));
        err = MNetError::No;
    } while (0);
    if (err != MNetError::No)
    {
        delete p_sock;
        return err;
    }
    listener_sock_list_.push_back(p_sock);
    return MNetError::No;
}

void MNetManager::OnAcceptCallback(size_t index)
{
    if (index >= listener_list_.size())
    {
        return;
    }
    MNetListener *p_listener = listener_event_list_[index].first;
    std::function<void (MNetConnector*)> callback = listener_event_list_[index].second;
    if (!p_listener || !callback)
    {
        return;
    }
    MSocket sock;
    while (p_listener->Accept(sock) == MNetError::No)
    {
        MNetEventHandler *p_handler = GetMinEventsEventHandler();
        if (!p_handler)
        {
            return;
        }
        MNetConnector *p_connector = new MNetConnector(sock.Detach(), *p_listener, *p_handler);
        if (p_connector)
        {
            callback(p_connector);
        }
    }
}

