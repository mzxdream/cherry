#include <iostream>
#include <mzx/convert.h>

#include <cmd/instance/cmd_scene.h>
#include <scene/instance/world_scene.h>
#include <world.h>

namespace cherry
{

CHERRY_CMD_REGIST(createworldscene, HandleCreateWorldScene);
CHERRY_CMD_REGIST(destroyscene, HandleDestroyScene);
CHERRY_CMD_REGIST(listscene, HandleListScene);
CHERRY_CMD_REGIST(selectscene, HandleSelectScene);

void HandleCreateWorldScene(const std::vector<std::string> &cmd)
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

void HandleDestroyScene(const std::vector<std::string> &cmd)
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

void HandleListScene(const std::vector<std::string> &cmd)
{
    auto &scene_manager = World::Instance().GetSceneManager();
    std::cout << "scene total count:" << scene_manager.SceneCount()
              << std::endl;
    scene_manager.ForeachScene([](Scene *scene) {
        std::cout << "->scene uuid:" << scene->UUID() << std::endl;
        return true;
    });
}

static SceneUUID g_select_scene_uuid = 0;
void HandleSelectScene(const std::vector<std::string> &cmd)
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
    g_select_scene_uuid = uuid;
    std::cout << "select scene:" << uuid << " success" << std::endl;
}

Scene *GetSelectScene()
{
    auto &scene_manager = World::Instance().GetSceneManager();
    auto *scene = scene_manager.GetScene(g_select_scene_uuid);
    if (!scene)
    {
        return nullptr;
    }
    return scene;
}

} // namespace cherry
