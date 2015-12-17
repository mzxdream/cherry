#include <net/m_socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

MSocket::MSocket()
    :sock_(-1)
    ,port_(0)
{
}

MSocket::~MSocket()
{
    Close();
}

MSocketError MSocket::Attach(int sock)
{
    if (sock >= 0 && sock_ != sock)
    {
        MSocketError ret = MSocketError::No;
        if ((ret = Close()) != MSocketError::No)
        {
            return ret;
        }
        sock_ = sock;
    }
    return MSocketError::No;
}

int MSocket::Detach()
{
    int old_sock = sock_;
    sock_ = -1;
    return old_sock;
}

MSocketError MSocket::Create(MSocketFamily family, MSocketType type, MSocketProtocol proto)
{
    if (sock_ >= 0)
    {
        return MSocketError::Created;
    }
    if ((sock_ = socket(static_cast<int>(family), static_cast<int>(type), static_cast<int>(proto))) == -1);
    {
        return CheckError();
    }
    return MSocketError::No;
}

MSocketError MSocket::Close()
{
    if (sock_ < 0)
    {
        if (close(sock_) == -1)
        {
            return CheckError();
        }
        sock_ = -1;
    }
    return MSocketError::No;
}

MSocketError MSocket::Bind(const std::string &ip, unsigned short port)
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
        return CheckError();
    }
    ip_ = ip;
    port_ = port;
    return MSocketError::No;
}

MSocketError MSocket::Listen(int count)
{
    if (listen(sock_, count) == -1)
    {
        return CheckError();
    }
    return MSocketError::No;
}

MSocketError MSocket::Accept(MSocket *p_sock)
{
    if (!p_sock)
    {
        return MSocketError::Invalid;
    }
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int sock = 0;
    if ((sock = accept(sock_, reinterpret_cast<struct sockaddr*>(&addr), &len)) == -1)
    {
        return CheckError();
    }
    MSocketError ret = p_sock->Attach(sock);
    if (ret != MSocketError::No)
    {
        close(sock);
        return ret;
    }
    p_sock->ip_->resize(64);
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, &((p_sock->ip_)[0]), p_sock->ip_->size());
    p_sock->port_ = ntohs(addr.sin_port);
    return MSocketError::No;
}

MSocketError MSocket::Connect(const std::string &ip, unsigned short port)
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
        return CheckError();
    }
    return MSocketError::No;
}

std::pair<int, MSocketError> MSocket::Send(const char *p_buf, int len)
{
    int send_len = 0;
    MSocketError err = MSocketError::No;
    if ((send_len = send(sock_, p_buf, len, 0)) == -1)
    {
        err = CheckError();
    }
    return std::make_pair(send_len, err);
}

std::pair<int, MSocketError> MSocket::Recv(void *p_buf, int len)
{
    int recv_len = 0;
    MSocketError err = MSocketError::No;
    if ((recv_len = recv(sock_, p_buf, len, 0)) == -1)
    {
        err = CheckError();
    }
    return std::make_pair(recv_len, err);
}

MSocketError MSocket::SetBlock(bool block)
{
    int flag = fcntl(sock_, F_GETFL, 0);
    if (flag < 0)
    {
        return CheckError();
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
        return CheckError();
    }
    return MSocketError::No;
}

MSocketError MSocket::SetReUseAddr(bool re_use)
{
    int reuse = re_use ? 1 : 0;
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse)) < 0)
    {
        return CheckError();
    }
    return MSocketError::No;
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

MSocketError MSocket::CheckError()
{
    if (errno == EINPROGRESS)
    {
        return MSocketError::InProgress;
    }
    else if (errno == EAGAIN
            || errno == EWOULDBLOCK)
    {
        return MSocketError::Again;
    }
    return MSocketError::Unknown;
}
