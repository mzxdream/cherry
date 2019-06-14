#include <iostream>

#include <cmd/instance/cmd_entity.h>
#include <cmd/instance/cmd_scene.h>
#include <ecs/component/health_point.h>
#include <scene/scene.h>

namespace cherry
{

CHERRY_CMD_REGIST(addentity, HandleAddEntity);
CHERRY_CMD_REGIST(removeentiy, HandleRemoveEntity);
CHERRY_CMD_REGIST(listentity, HandleListEntity);
CHERRY_CMD_REGIST(infoentity, HandleInfoEntity);
CHERRY_CMD_REGIST(addcomponent, HandleAddComponent);

void HandleAddEntity(const std::vector<std::string> &cmd)
{
    auto *scene = GetSelectScene();
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

void HandleRemoveEntity(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "remove entity cmd size < 2" << std::endl;
        return;
    }
    auto *scene = GetSelectScene();
    if (!scene)
    {
        std::cout << "get select scene failed" << std::endl;
        return;
    }
    auto uuid = mzx::UnsafeConvertTo<mzx::EntityID>(cmd[1]);
    scene->GetEntityManager().RemoveEntity(uuid);
    std::cout << "remove entity:" << uuid << " success" << std::endl;
}

void HandleListEntity(const std::vector<std::string> &cmd)
{
    auto *scene = GetSelectScene();
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

void HandleInfoEntity(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 2)
    {
        std::cout << "info entity cmd size < 2" << std::endl;
        return;
    }
    auto *scene = GetSelectScene();
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

void HandleAddComponent(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 3)
    {
        std::cout << "add component cmd size < 3" << std::endl;
        return;
    }
    auto *scene = GetSelectScene();
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
    std::string data;
    if (cmd.size() >= 4)
    {
        data = cmd[3];
    }
    auto *component =
        ComponentSerializeFactory::Instance().Unserialize(cmd[2].c_str(), data);
    if (!component)
    {
        std::cout << "create component " << cmd[2] << " failed" << std::endl;
        return;
    }
    if (!entity->AddComponent(component))
    {
        std::cout << "add component " << cmd[2] << " failed" << std::endl;
        return;
    }
    delete component;
    std::cout << "add component " << cmd[2] << " success" << std::endl;
}

} // namespace cherry
