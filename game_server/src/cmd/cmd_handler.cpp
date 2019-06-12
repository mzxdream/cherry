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

static void HandleExit(const std::vector<std::string> &cmd)
{
    World::Instance().Stop();
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

static void HandleListScene(const std::vector<std::string> &cmd)
{
    auto &scene_manager = World::Instance().GetSceneManager();
    std::cout << "scene total count:" << scene_manager.SceneCount()
              << std::endl;
    scene_manager.ForeachScene([](Scene *scene) {
        std::cout << "scene uuid:" << scene->UUID() << std::endl;
        return true;
    });
}

static SceneUUID select_scene_uuid = 0;
static void HandleSelectScene(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "select scene cmd size < 2" << std::endl;
        return;
    }
    auto uuid = mzx::UnsafeConvertTo<SceneUUID>(cmd[1]);
    auto &scene_manager = World::Instance().GetSceneManager();
    if (!scene_manager.GetScene(uuid))
    {
        std::cout << "scene:" << uuid << " not exist" << std::endl;
        return;
    }
    select_scene_uuid = uuid;
    std::cout << "select scene:" << uuid << " success" << std::endl;
}

void CmdHandler::Regist()
{
    mzx::CmdLine::Regist(HandleCmd);
    mzx::CmdLine::Regist("exit", HandleExit);
    mzx::CmdLine::Regist("createworldscene", HandleCreateWorldScene);
    mzx::CmdLine::Regist("destroyscene", HandleDestroyScene);
    mzx::CmdLine::Regist("listscene", HandleListScene);
    mzx::CmdLine::Regist("selectscene", HandleSelectScene);
}

} // namespace cherry
