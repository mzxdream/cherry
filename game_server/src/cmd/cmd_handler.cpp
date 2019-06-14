#include <iostream>
#include <mzx/convert.h>
#include <mzx/system/cmd_line.h>

#include <cmd/cmd_handler.h>
#include <world.h>

namespace cherry
{

static void HandleDefaultCmd(const std::vector<std::string> &cmd)
{
    if (cmd.empty())
    {
        return;
    }
    std::cout << cmd[0] << " handle not found" << std::endl;
}

static void HandleExit(const std::vector<std::string> &cmd)
{
    World::Instance().Stop();
}

CmdHandler::CmdHandler()
{
    mzx::CmdLine::Regist(HandleDefaultCmd);
    mzx::CmdLine::Regist("exit", HandleExit);
}

CmdHandler::~CmdHandler()
{
    mzx::CmdLine::UnregistAll();
}

Scene *CmdHandler::GetSelectScene()
{
    return World::Instance().GetSceneManager().GetScene(select_scene_uuid_);
}

void CmdHandler::SetSelectSceneUUID(SceneUUID uuid)
{
    select_scene_uuid_ = uuid;
}

bool CmdHandler::Start()
{
    return mzx::CmdLine::Start();
}

void CmdHandler::Stop()
{
    mzx::CmdLine::Stop();
}

bool CmdHandler::Regist(
    const char *name, std::function<void(const std::vector<std::string> &)> cb)
{
    if (!name)
    {
        return false;
    }
    mzx::CmdLine::Regist(name, cb);
    return true;
}

} // namespace cherry
