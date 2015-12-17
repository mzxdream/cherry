#ifndef _M_EPOLL_H_
#define _M_EPOLL_H_

enumc class MEpollError
{
    No = 0,
    Unknown = 1,
    Created = 2,
};

class MEpoll
{
public:
    MEpoll();
    ~MEpoll();
    MEpoll(const MEpoll &) = delete;
    MEpoll& operator=(const MEpoll &) = delete;
public:
    MEpollError Attach(int fd);
    int Detach();
    MEpollError Create();
    MEpollError Close();
    MEpollError AddEvent();
    MEpollError DelEvent();
    MEpollError CloseEvent();
private:
    int fd_;
};

#endif
