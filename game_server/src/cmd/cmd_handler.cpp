#include <iostream>
#include <mzx/convert.h>
#include <mzx/system/cmd_line.h>

#include <cmd/cmd_handler.h>
#include <scene/instance/world_scene.h>
#include <world.h>

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

static void HandleCreateWorldScene(const std::vector<std::string> &cmd)
{
    auto &scene_manager = World::Instance().GetSceneManager();
    auto *scene = scene_manager.CreateScene<WorldScene>();
    if (!scene)
    {
        std::cout << "create world scene failed" << std::endl;
        return;
    }
    std::cout << "create world scene:" << scene->UUID() << " success"
              << std::endl;
}

static void HandleDestroyScene(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "destroy scene cmd size < 2" << std::endl;
        return;
    }
    auto &scene_manager = World::Instance().GetSceneManager();
    scene_manager.DestroyScene(mzx::UnsafeConvertTo<SceneUUID>(cmd[1]));
    std::cout << "destroy scene success" << std::endl;
}

void CmdHandler::Regist()
{
    mzx::CmdLine::Regist(HandleCmd);
    mzx::CmdLine::Regist("createworldscene", HandleCreateWorldScene);
    mzx::CmdLine::Regist("destroyscene", HandleDestroyScene);
}

} // namespace cherry
