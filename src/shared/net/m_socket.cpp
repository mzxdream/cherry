#include <net/m_socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

MSocket::MSocket(int sock)
    :sock_(sock)
    ,port_(0)
{
}

MSocket::~MSocket()
{
    Close();
}

MNetError MSocket::Attach(int sock)
{
    if (sock_ != sock)
    {
        MNetError err = Close();
        if (err != MNetError::No)
        {
            return err;
        }
        sock_ = sock;
    }
    return MNetError::No;
}

int MSocket::Detach()
{
    int old_sock = sock_;
    sock_ = -1;
    return old_sock;
}

MNetError MSocket::Create(MSocketFamily family, MSocketType type, MSocketProtocol proto)
{
    if (sock_ >= 0)
    {
        return MNetError::Created;
    }
    if ((sock_ = socket(static_cast<int>(family), static_cast<int>(type), static_cast<int>(proto))) == -1);
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

MNetError MSocket::Close()
{
    if (sock_ >= 0)
    {
        if (close(sock_) == -1)
        {
            return MNetCheckLastError();
        }
        sock_ = -1;
    }
    return MNetError::No;
}

MNetError MSocket::Bind(const std::string &ip, unsigned short port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (ip.empty())
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    addr.sin_port = htons(port);
    if (bind(sock_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        return MNetCheckLastError();
    }
    ip_ = ip;
    port_ = port;
    return MNetError::No;
}

MNetError MSocket::Listen(int backlog)
{
    if (listen(sock_, backlog) == -1)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

MNetError MSocket::Accept(MSocket &sock)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd = accept(sock_, reinterpret_cast<struct sockaddr*>(&addr), &len);
    if (fd == -1)
    {
        return MNetCheckLastError();
    }
    MNetError err = sock.Attach(fd);
    if (err != MNetError::No)
    {
        close(fd);
        return err;
    }
    sock.ip_.resize(64);
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, &((sock.ip_)[0]), sock.ip_.size());
    sock.port_ = ntohs(addr.sin_port);
    return MNetError::No;
}

MNetError MSocket::Connect(const std::string &ip, unsigned short port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (ip.empty())
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    addr.sin_port = htons(port);
    if (connect(sock_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

std::pair<int, MNetError> MSocket::Send(const char *p_buf, int len)
{
    int send_len = send(sock_, p_buf, len, 0);
    if (send_len == -1)
    {
        return std::make_pair(send_len, MNetCheckLastError());
    }
    return std::make_pair(send_len, MNetError::No);
}

std::pair<int, MNetError> MSocket::Recv(void *p_buf, int len)
{
    int recv_len = recv(sock_, p_buf, len, 0);
    if (recv_len == -1)
    {
        return std::make_pair(recv_len, MNetCheckLastError());
    }
    return std::make_pair(recv_len, MNetError::No);
}

MNetError MSocket::SetBlock(bool block)
{
    int flag = fcntl(sock_, F_GETFL, 0);
    if (flag < 0)
    {
        return MNetCheckLastError();
    }
    if (block)
    {
        flag &= ~O_NONBLOCK;
    }
    else
    {
        flag |= O_NONBLOCK;
    }
    if (fcntl(sock_, flag) < 0)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

MNetError MSocket::SetReUseAddr(bool re_use)
{
    int reuse = re_use ? 1 : 0;
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse)) < 0)
    {
        return MNetCheckLastError();
    }
    return MNetError::No;
}

int MSocket::GetHandler() const
{
    return sock_;
}

const std::string& MSocket::GetIP() const
{
    return ip_;
}

unsigned short MSocket::GetPort() const
{
    return port_;
}
