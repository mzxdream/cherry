#ifndef _M_SOCKET_H_
#define _M_SOCKET_H_

#include <net/m_net_common.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

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
}

class MSocket
{
public:
    MSocket();
    ~MSocket();
    MSocket(const MSocket &) = delete;
    MSocket& operator=(const MSocket &) = delete;
public:
    MNetError Init(MSocketFamily family, MSocketType type, MSocketProtocol proto);
    void Close();
    MNetError Bind(const std::string &ip, unsigned short port);
private:
    int sock_;
};

#endif
