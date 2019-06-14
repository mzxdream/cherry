#include <iostream>
#include <mzx/convert.h>

#include <scene/instance/world_scene.h>
#include <world.h>
#include <cmd/cmd_handler.h>

namespace cherry
{

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
CHERRY_CMD_REGIST(createworldscene, HandleCreateWorldScene);

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
CHERRY_CMD_REGIST(destroyscene, HandleDestroyScene);

static void HandleListScene(const std::vector<std::string> &cmd)
{
    auto &scene_manager = World::Instance().GetSceneManager();
    std::cout << "scene total count:" << scene_manager.SceneCount()
              << std::endl;
    scene_manager.ForeachScene([](Scene *scene) {
        std::cout << "->scene uuid:" << scene->UUID() << std::endl;
        return true;
    });
}
CHERRY_CMD_REGIST(listscene, HandleListScene);

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
    CmdHandler::Instance().SetSelectSceneUUID(uuid);
    std::cout << "select scene:" << uuid << " success" << std::endl;
}
CHERRY_CMD_REGIST(selectscene, HandleSelectScene);

} // namespace cherry
