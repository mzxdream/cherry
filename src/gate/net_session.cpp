#include <net_session.h>

NetSession::NetSession()
    :p_connector_(nullptr)
    ,p_event_loop_thread_(nullptr)
    ,recv_pkg_len_(0)
    ,recv_pkg_len_read_(false)
{
}

NetSession::~NetSession()
{
    if (p_connector_)
    {
        delete p_connector_;
    }
}

void NetSession::SetConnector(MNetConnector *p_connector)
{
    p_connector_ = p_connector;
}

MNetConnector NetSession::GetConnector()
{
    return p_connector_;
}

void NetSession::SetEventLoopThread(MNetEventLoopThread *p_event_loop_thread)
{
    p_event_loop_thread_ = p_event_loop_thread;
}

MNetEventLoopThread* NetSession::GetEventLoopThread()
{
    return p_event_loop_thread_;
}

bool NetSession::SendNetPkg(const NetPackage &pkg)
{
    if (!p_connector_)
    {
        return false;
    }
    std::lock_guard<std::mutex> lock(send_pkg_mtx_);
    send_pkg_list_.push_back(pkg);
    return true;
}

void NetSession::Update()
{
}

void
