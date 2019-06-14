#include <iostream>
#include <mzx/convert.h>
#include <ecs/helper/component_serialize.h>

#include <cmd/cmd_handler.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

static void HandleAddComponent(const std::vector<std::string> &cmd)
{
    if (cmd.size() < 3)
    {
        std::cout << "add component cmd size < 3" << std::endl;
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
CHERRY_CMD_REGIST(addcomponent, HandleAddComponent);

} // namespace cherry
