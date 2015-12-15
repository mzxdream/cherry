#ifndef _M_EVENT_HANDLER_H_
#define _M_EVENT_HANDLER_H_

#include <functional>

enum class MEventType
{

};

class MEventHandler;

class MEvent
{
public:
    explicit MEvent(MEventHandler *p_handler, int fd, int ev, const std::function<void (int,)> &cb);
    ~MEvent();
private:
    MEventHandler *p_handler_;
    int fd_;
    int ev_;
    std::function<void (int, int)> cb_;
};

class MEventHandler
{
public:
    MEventHandler();
    ~MEventHandler();
    MEventHandler(const MEventHandler &) = delete;
    MEventHandler& operator=(const MEventHandler &) = delete;
private:

};

#endif
