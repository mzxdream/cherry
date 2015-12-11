#ifndef _M_EVENT_H_
#define _M_EVENT_H_

enum class MEventError
{
};

class MEvent
{
public:
    MEvent();
    virtual ~MEvent();
    MEvent(const MEvent &) = delete;
    MEvent& operator=(const MEvent &) = delete;
public:
    MEventError AsyncAccept();
    MEventError AsyncConnect();
    MEventError AysncReadSome();
    MEventError AsyncWriteSome();
private:
};

#endif
