#include <net/m_epoll.h>
#include <sys/epoll.h>

MEpoll::MEpoll()
    :fd_(-1)
{
}

MEpoll::~MEpoll()
{
    Close();
}

MEpollError MEpoll::Attach(int fd)
{
    if (fd >= 0 && fd_ != fd)
    {
        MEpollError ret = MEpollError::No;
        if ((ret = Close()) != MEpollError::No)
        {
            return ret;
        }
        fd_ = fd;
    }
    return MEpollError::No;
}

int MSocket::Detach()
{
    int old_fd = fd_;
    fd_ = -1;
    return old_fd;
}

MEpollError MEpoll::Create()
{
    if (fd_ >= 0)
    {
        return MEpollError::Created;
    }
    if ((fd_ = epoll_create1(0)) == -1)
    {
        return CheckError();
    }
    return MEpollError::No;
}

MEpollError MEpoll::Close()
{
    if (fd_ < 0)
    {
        if (close(fd_) == -1)
        {
            return CheckError();
        }
        fd_ = -1;
    }
    return MEpollError::No;
}

MEpollError MEpoll::AddEvent()
{
}

MEpollError MEpoll::DelEvent()
{
}
