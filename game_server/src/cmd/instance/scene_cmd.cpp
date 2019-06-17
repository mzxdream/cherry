#include <iostream>
#include <mzx/convert.h>

#include <cmd/cmd_handler.h>
#include <ecs/component/serialize/component_serialize.h>
#include <scene/instance/world_scene.h>
#include <world.h>

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

static void HandleInfoScene(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "info scene cmd size < 2" << std::endl;
        return;
    }
    auto uuid = mzx::UnsafeConvertTo<SceneUUID>(cmd[1]);
    auto &scene_manager = World::Instance().GetSceneManager();
    auto *scene = scene_manager.GetScene(uuid);
    if (!scene)
    {
        std::cout << "scene:" << uuid << " not exist" << std::endl;
        return;
    }
    std::cout << "entity count:" << scene->GetEntityManager().EntityCount()
              << std::endl;
    scene->GetEntityManager().ForeachEntity([](mzx::Entity *entity) {
        std::cout << "entity:" << entity->ID() << std::endl;
        entity->ForeachComponent([](mzx::ComponentBase *base) {
            std::cout << "->index:" << base->ClassIndex();
            std::string data;
            const char *name =
                ComponentSerializeFactory::Instance().Serialize(base, &data);
            if (name)
            {
                std::cout << " name:" << name << " data:" << data;
            }
            else
            {
                std::cout << " name: ? data: ?";
            }
            std::cout << std::endl;
            return true;
        });
        return true;
    });
}
CHERRY_CMD_REGIST(infoscene, HandleInfoScene);

} // namespace cherry
