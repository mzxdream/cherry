#ifndef _M_TCP_SOCKET_H_
#define _M_TCP_SOCKET_H_

class MTcpSocket
{
public:
    MTcpSocket();
    ~MTcpSocket();
    MTcpSocket(const MTcpSocket &) = delete;
    MTcpSocket& operator=(const MTcpSocket &) = delete;
public:

private:

private:
    int sock_;
};

#endif
