#ifndef _M_EPOLL_H_
#define _M_EPOLL_H_

#include <net/m_net_event.h>
#include <vector>

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
    MEpollError Create(size_t max_events);
    MEpollError Close();
    MEpollError AddEvent(int sock, int event, MNetEvent &ev);
    MEpollError DelEvent(int sock, int event, MNetEvent &ev);
    MEpollError CloseEvent(int sock, MNetEvent &ev);
    MEpollError ProcessEvents(int timeout);
private:
    MEpollError CheckError();
private:
    int fd_;
    std::vector<epoll_event> event_list_;
};

#endif
