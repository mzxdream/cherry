#include <net/m_net_event_loop_thread.h>
#include <net/m_net_connector.h>
#include <net/m_net_listener.h>
#include <util/m_logger.h>

uint16_t len = 0;
bool len_readed = false;

void OnConnectCallback(MNetConnector *p_connector)
{
    MLOG(MGetLibLogger(), MERR, "connect success");
}

void OnErrorCallback(MError err)
{
    MLOG(MGetLibLogger(), MERR, "err:", static_cast<int>(err));
}

void OnReadCallback(MNetConnector *p_connector)
{

}

void OnWriteCompleteCallback(MNetConnector *p_connector)
{

}

int main (int argc, char *argv[])
{
    MNetEventLoopThread ev_thread;
    MError err = ev_thread.Init();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "event loop thread init failed");
        return 0;
    }
    err = ev_thread.Start();
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "event loop thread start failed");
        return 0;
    }
    MSocket sock;
    err = sock.Create(MSocketFamily::IPV4, MSocketType::TCP, MSocketProtocol::Default);
    if (err != MError::No)
    {
        MLOG(MGetLibLogger(), MERR, "create socket failed");
        return 0;
    }
    MNetConnector conn(&sock, nullptr, &ev_thread.GetEventLoop(), nullptr, nullptr, nullptr, nullptr, false, 10, 10);
    conn.SetConnectCallback(std::bind(OnConnectCallback, &conn));
    conn.SetReadCallback(std::bind(OnReadCallback, &conn));
    conn.SetWriteCompleteCallback(std::bind(OnWriteCompleteCallback, &conn));
    conn.SetErrorCallback(std::bind(OnErrorCallback));

    return 0;
}

