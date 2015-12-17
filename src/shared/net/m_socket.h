#ifndef _M_SOCKET_H_
#define _M_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <utility>

enum class MSocketError
{
    No = 0,
    Unknown = 1,
    Created = 2,
    Invalid = 3,
    AddrToStrFailed = 4,
    InProgress = 5,
    Again = 6,
};

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
    MSocket();
    ~MSocket();
    MSocket(const MSocket &) = delete;
    MSocket& operator=(const MSocket &) = delete;
public:
    MSocketError Attach(int sock);
    int Detach();
    MSocketError Create(MSocketFamily family, MSocketType type, MSocketProtocol proto);
    MSocketError Close();
    MSocketError Bind(const std::string &ip, unsigned short port);
    MSocketError Listen(int count);
    MSocketError Accept(MSocket *p_sock);
    MSocketError Connect(const std::string &ip, unsigned short port);
    std::pair<int, MSocketError> Send(const char *p_buf, int len);
    std::pair<int, MSocketError> Recv(void *p_buf, int len);
    MSocketError SetBlock(bool block);
    MSocketError SetReUseAddr(bool re_use);
    int GetHandler() const;
    const std::string& GetIP() const;
    unsigned short GetPort() const;
private:
    MSocketError CheckError();
private:
    int sock_;
    std::string ip_;
    unsigned short port_;
};

#endif
