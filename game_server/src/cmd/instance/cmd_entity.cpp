#include <iostream>
#include <mzx/convert.h>

#include <cmd/cmd_handler.h>
#include <ecs/helper/component_serialize.h>
#include <scene/scene.h>

namespace cherry
{

static void HandleAddEntity(const std::vector<std::string> &cmd)
{
    auto *scene = CmdHandler::Instance().GetSelectScene();
    if (!scene)
    {
        std::cout << "get select scene failed" << std::endl;
        return;
    }
    auto *entity = scene->GetEntityManager().AddEntity();
    if (!entity)
    {
        std::cout << "add entity failed" << std::endl;
        return;
    }
    std::cout << "add entity:" << entity->ID() << std::endl;
}
CHERRY_CMD_REGIST(addentity, HandleAddEntity);

static void HandleRemoveEntity(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "remove entity cmd size < 2" << std::endl;
        return;
    }
    auto *scene = CmdHandler::Instance().GetSelectScene();
    if (!scene)
    {
        std::cout << "get select scene failed" << std::endl;
        return;
    }
    auto uuid = mzx::UnsafeConvertTo<mzx::EntityID>(cmd[1]);
    scene->GetEntityManager().RemoveEntity(uuid);
    std::cout << "remove entity:" << uuid << " success" << std::endl;
}
CHERRY_CMD_REGIST(removeentiy, HandleRemoveEntity);

static void HandleListEntity(const std::vector<std::string> &cmd)
{
    auto *scene = CmdHandler::Instance().GetSelectScene();
    if (!scene)
    {
        std::cout << "get select scene failed" << std::endl;
        return;
    }
    std::cout << "entity total count:"
              << scene->GetEntityManager().EntityCount() << std::endl;
    scene->GetEntityManager().ForeachEntity([](const mzx::Entity *entity) {
        std::cout << "->entity:" << entity->ID() << std::endl;
        return true;
    });
}
CHERRY_CMD_REGIST(listentity, HandleListEntity);

static void HandleInfoEntity(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "info entity cmd size < 2" << std::endl;
        return;
    }
    auto *scene = CmdHandler::Instance().GetSelectScene();
    if (!scene)
    {
        std::cout << "get select scene failed" << std::endl;
        return;
    }
    auto uuid = mzx::UnsafeConvertTo<mzx::EntityID>(cmd[1]);
    auto *entity = scene->GetEntityManager().GetEntity(uuid);
    if (!entity)
    {
        std::cout << "entity:" << uuid << " not exist" << std::endl;
        return;
    }
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
}
CHERRY_CMD_REGIST(infoentity, HandleInfoEntity);

} // namespace cherry
