#ifndef __CHERRY_CMD_EVENT_H__
#define __CHERRY_CMD_EVENT_H__

#include <string>
#include "ecs/event/event.h"

namespace cherry {

class CmdEvent
    : public Event
{
public:
    CmdEvent(const std::string &cmd)
        : cmd_(cmd)
    {
    }
public:
    const std::string & GetCmd() const
    {
        return cmd_;
    }
private:
    std::string cmd_;
};

}

#endif
