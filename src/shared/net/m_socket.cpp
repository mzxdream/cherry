#include <net/m_socket.h>
#include <string.h>

MSocket::MSocket()
    :sock_(-1)
{
}

MSocket::~MSocket()
{
    Close();
}

MNetError MSocket::Init(MSocketFamily family, MSocketType type, MSocketProtocol proto)
{
    if (sock_ < 0)
    {
        return MNetError::SocketCreated;
    }
    sock_ = socket(static_cast<int>(family), static_cast<int>(type), static_cast<int>(proto));
    if (sock_ < 0)
    {
        return MNetError::SocketCreateFailed;
    }
    return MNetError::No;
}

void MSocket::Close()
{
    if (sock_ < 0)
    {
        close(sock_);
        sock_ = -1;
    }
}

MNetError MSocket::Bind(const std::string &ip, unsigned short port)
{

}
