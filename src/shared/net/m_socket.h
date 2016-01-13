#ifndef _M_SOCKET_H_
#define _M_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <utility>
#include <net/m_net_common.h>

enum class MSocketFamily
{
    IPV4 = AF_INET,
    IPV6 = AF_INET6,
};

enum class MSocketType
{
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
};

enum class MSocketProtocol
{
    Default = 0,
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP,
};

class MSocket
{
public:
    explicit MSocket(int sock = -1);
    ~MSocket();
    MSocket(const MSocket &) = delete;
    MSocket& operator=(const MSocket &) = delete;
public:
    MNetError Attach(int sock);
    int Detach();
    MNetError Create(MSocketFamily family, MSocketType type, MSocketProtocol proto);
    MNetError Close();
    MNetError Bind(const std::string &ip, unsigned short port);
    MNetError Listen(int backlog);
    MNetError Accept(MSocket &sock);
    MNetError Connect(const std::string &ip, unsigned short port);
    std::pair<int, MNetError> Send(const char *p_buf, int len);
    std::pair<int, MNetError> Recv(void *p_buf, int len);
    MNetError SetBlock(bool block);
    MNetError SetReUseAddr(bool re_use);
    int GetHandler() const;
    const std::string& GetIP() const;
    unsigned short GetPort() const;
public:
    MNetError CreateNonblockReuseListener(const std::string &ip, unsigned short port, int backlog);
private:
    int sock_;
    std::string ip_;
    unsigned short port_;
};

#endif
