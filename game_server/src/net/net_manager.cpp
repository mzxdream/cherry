#include <net/net_manager.h>

namespace cherry
{

class NetConnector final
{
public:
    explicit NetConnector(asio::ip::tcp::socket sock);
    ~NetConnector();
    NetConnector(const NetConnector &) = delete;
    NetConnector &operator=(const NetConnector &) = delete;

public:
    void SendMessage(const char *data, std::size_t size);

private:
    asio::ip::tcp::socket sock_;
    uint16_t packet_size_;
};

NetAcceptor::NetAcceptor(NetManager &net_manager, int port)
    : net_manager_(net_manager)
{
}

NetAcceptor::~NetAcceptor()
{
}

void NetAcceptor::DoAccept()
{
    acceptor_.async_accept(socket_, [this](std::error error) {
        if (!error)
        {
            net_manager_.Post();
        }
        DoAccept();
    });
}

NetManager::NetManager()
    : io_service_work_(io_service_)
    , service_thread_(std::bind(&aio::io_service::run, &io_service_))
{
}

NetManager::~NetManager()
{
}

void NetManager::Start()
{
    service_thread_.Start();
}

void NetManager::Stop()
{
    io_service_.stop();
}

void NetManager::Join()
{
    service_thread_.Join();
}

void NetManager::AddAcceptor(NetAcceptor *acceptor)
{
    if (!acceptor || acceptor_list_.find(acceptor) != acceptor_list_.end())
    {
        return;
    }
    acceptor_list_.insert(acceptor);
}

void NetManager::RemoveAcceptor(NetAcceptor *acceptor)
{
}

void NetManager::AddPeerConnector(NetPeerConnector *peer_connector)
{
}

void NetManager::RemovePeerConnector(NetPeerConnector *peer_connector)
{
}

void NetManager::SendMessage(NetID id, const char *data, std::size_t size)
{
}

} // namespace cherry
