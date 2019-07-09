#ifndef __CHERRY_NET_MANAGER_H__
#define __CHERRY_NET_MANAGER_H__

#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <string>

#include <mzx/thread.h>

namespace cherry
{

using NetID = int;
constexpr NetID NET_ID_INVALID = -1;

class NetAcceptor
{
protected:
    NetAcceptor(NetManager &net_manager, int port);
    virtual ~NetAcceptor();

private:
    void DoAccept();
    void OnNewConnect(NetID id);

private:
    virtual void _OnNewConnect(NetID id)
    {
    }
    virtual void _OnDisconnect(NetID id)
    {
    }
    virtual void _OnRecvMessage(NetID id, const char *data, std::size_t)
    {
    }

private:
    NetManager &net_manager_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket sock_;
};

class NetPeerConnector
{
public:
    NetPeerConnector(const std::string &ip, int port);
    virtual ~NetPeerConnector();

private:
    virtual void _OnConnect(NetID id, std::error_code error)
    {
    }
    virtual void _OnDisconnect(NetID id)
    {
    }
    virtual void _OnRecvMessage(NetID id, const char *data, std::size_t size)
    {
    }
};

class NetConnector
{
};

class NetManager final
{
public:
    NetManager();
    ~NetManager();
    NetManager(const NetManager &) = delete;
    NetManager &operator=(const NetManager &) = delete;

public:
    void Start();
    void Stop();
    void Join();
    void Post(std::function<void()> func);

    void AddAcceptor(NetAcceptor *acceptor);
    void RemoveAcceptor(NetAcceptor *acceptor);
    void AddPeerConnector(NetPeerConnector *peer_connector);
    void RemovePeerConnector(NetPeerConnector *peer_connector);

    void SendMessage(NetID id, const char *data, std::size_t size);
    void Close(NetID id);

private:
    std::set<NetAcceptor *> acceptor_list_;
    std::set<NetPeerConnector *> peer_connector_list_;
    std::unordered_map<NetID, NetConnector *> connector_list_;
    asio::io_service io_service_;
    asio::io_service::work io_service_work_;
    mzx::Thread service_thread_;
};

} // namespace cherry

#endif
