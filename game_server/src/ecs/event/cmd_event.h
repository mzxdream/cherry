#ifndef __CHERRY_CMD_EVENT_H__
#define __CHERRY_CMD_EVENT_H__

#include <string>

namespace cherry {

struct CmdEvent
{
    CmdEvent(const std::string &_cmd)
        : cmd(_cmd)
    {
    }
    std::string cmd;
};

}

#endif
