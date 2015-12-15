#ifndef _EVENT_SOURCE_H_
#define _EVENT_SOURCE_H_

class MEventSource
{
public:
    MEventSource();
    virtual ~MEventSource();
    MEventSource(const MEventSource &) = delete;
    MEventSource& operator=(const MEventSource &) = delete;
public:
private:
};

#endif
