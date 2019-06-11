#include <mzx/system/cmd_line.h>

#include <cmd/cmd_handler.h>

namespace cherry
{

static void HandleCmd(const std::vector<std::string> &cmd)
{
    if (cmd.empty())
    {
        return;
    }
    std::cout << "cmd:" << cmd[0] << " handle not found" << std::endl;
}

void CmdHandler::Regist()
{
    mzx::CmdLine::Regist(HandleCmd);
    // TODO
}

} // namespace cherry
