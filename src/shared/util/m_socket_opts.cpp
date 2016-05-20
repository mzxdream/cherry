#include <net/m_socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

std::pair<int, MError> MSocketOpts::Create(MSocketFamily family, MSocketType type, MSocketProto proto)
{
    MError err = MError::No;
    int sock = socket(static_cast<int>(family), static_cast<int>(type), static_cast<int>(proto));
    if (sock == -1)
    {
        err = MError::Unknown;
    }
    return std::make_pair(sock, err);
}

MError MSocketOpts::Destroy(int sock)
{
    if (sock < 0)
    {
        return MError::No;
    }
    if (close(sock_) == -1)
    {
        return MError::Unknown;
    }
    return MError::No;
}

MError MSocketOpts::Bind(int sock, const std::string &ip, unsigned port)
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
    if (bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        return MError::Unknown;
    }
    return MError::No;
}

MError MSocketOpts::Listen(int sock, int backlog)
{
    if (listen(sock, backlog) == -1)
    {
        return MError::Unknown;
    }
    return MError::No;
}

MError MSocketOpts::Accept(int sock, int &accept_sock)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    accept_sock = accept(sock_, reinterpret_cast<struct sockaddr*>(&addr), &len);
    if (fd == -1)
    {
        if (errno == EINTR)
        {
            return MError::InterruptedSysCall;
        }
        else if (errno == EAGAIN)
        {
            return MError::Again;
        }
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return MError::Unknown;
    }
    MError err = sock.Attach(fd);
    if (err != MError::No)
    {
        close(fd);
        return err;
    }
    sock.remote_ip_.resize(64);
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, &((sock.remote_ip_)[0]), sock.remote_ip_.size());
    sock.remote_port_ = ntohs(addr.sin_port);
    return MError::No;
}

MError MSocketOpts::Connect(const std::string &ip, unsigned port)
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
        if (errno == EINPROGRESS)
        {
            return MError::InProgress;
        }
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return MError::Unknown;
    }
    remote_ip_ = ip;
    remote_port_ = port;
    return MError::No;
}

std::pair<int, MError> MSocketOpts::Send(const char *p_buf, int len)
{
    if (len <= 0)
    {
        return std::make_pair(0, MError::No);
    }
    int send_len = send(sock_, p_buf, len, 0);
    if (send_len == -1)
    {
        if (errno == EINTR)
        {
            return std::make_pair(0, MError::InterruptedSysCall);
        }
        else if (errno == EAGAIN)
        {
            return std::make_pair(0, MError::Again);
        }
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return std::make_pair(0, MError::Unknown);
    }
    return std::make_pair(send_len, MError::No);
}

std::pair<int, MError> MSocketOpts::Recv(void *p_buf, int len)
{
    int recv_len = recv(sock_, p_buf, len, 0);
    if (recv_len == -1)
    {
        if (errno == EINTR)
        {
            return std::make_pair(0, MError::InterruptedSysCall);
        }
        else if (errno == EAGAIN)
        {
            return std::make_pair(0, MError::Again);
        }
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return std::make_pair(0, MError::Unknown);
    }
    return std::make_pair(recv_len, MError::No);
}

MError MSocketOpts::SetBlock(bool block)
{
    int flag = fcntl(sock_, F_GETFL, 0);
    if (flag == -1)
    {
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return MError::Unknown;
    }
    if (block)
    {
        flag &= ~O_NONBLOCK;
    }
    else
    {
        flag |= O_NONBLOCK;
    }
    flag = fcntl(sock_, F_SETFL, flag);
    if (flag == -1)
    {
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return MError::Unknown;
    }
    return MError::No;
}

MError MSocketOpts::SetReUseAddr(bool re_use)
{
    int reuse = re_use ? 1 : 0;
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse)) < 0)
    {
        MLOG(MGetLibLogger(), MERR, "errno is ", errno);
        return MError::Unknown;
    }
    return MError::No;
}

int MSocketOpts::GetHandler() const
{
    return sock_;
}

const std::string& MSocketOpts::GetBindIP() const
{
    return bind_ip_;
}

unsigned MSocketOpts::GetBindPort() const
{
    return bind_port_;
}

const std::string& MSocketOpts::GetRemoteIP() const
{
    return remote_ip_;
}

unsigned MSocketOpts::GetRemotePort() const
{
    return remote_port_;
}

MError MSocketOpts::CreateNonblockReuseAddrListener(const std::string &ip, unsigned short port, int backlog)
{
    MError err = Create(MSocketFamily::IPV4, MSocketType::TCP, MSocketProtocol::Default);
    if (err != MError::No)
    {
        return err;
    }
    err = SetBlock(false);
    if (err != MError::No)
    {
        return err;
    }
    err = SetReUseAddr(true);
    if (err != MError::No)
    {
        return err;
    }
    err = Bind(ip, port);
    if (err != MError::No)
    {
        return err;
    }
    err = Listen(backlog);
    if (err != MError::No)
    {
        return err;
    }
    return MError::No;
}
