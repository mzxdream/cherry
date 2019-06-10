#ifndef __CHERRY_CMD_EVENT_H__
#define __CHERRY_CMD_EVENT_H__

#include <mzx/simple_event.h>
#include <string>

namespace cherry
{

struct CmdEvent : public mzx::SimpleEvent<CmdEvent>
{
    explicit CmdEvent(const std::string &command)
        : cmd(command)
    {
    }
    std::string cmd;
};

} // namespace cherry

#endif
