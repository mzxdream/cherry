#include <iostream>

#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/single/scene_grid_view.h>
#include <ecs/event/component_event.h>
#include <ecs/system/scene_grid_view_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

SceneGridViewSystem::SceneGridViewSystem(Scene *scene)
    : scene_(scene)
{
    if (scene_)
    {
        scene_->GetEventManager().AddListener<ComponentAddEvent>(
            std::bind(&SceneGridViewSystem::OnAddComponent, this,
                      std::placeholders::_1),
            this);
        scene_->GetEventManager().AddListener<ComponentRemoveEvent>(
            std::bind(&SceneGridViewSystem::OnRemoveComponent, this,
                      std::placeholders::_1),
            this);
        scene_->GetSceneEntity().AddComponent<SceneGridView>(100.0);
    }
}

SceneGridViewSystem::~SceneGridViewSystem()
{
    if (scene_)
    {
        scene_->GetSceneEntity().RemoveComponent<SceneGridView>();
        scene_->GetEventManager().RemoveListener<ComponentAddEvent>(this);
        scene_->GetEventManager().RemoveListener<ComponentRemoveEvent>(this);
    }
}

void SceneGridViewSystem::OnAddComponent(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const ComponentAddEvent *>(base);
    std::cout << "OnAddComponent entity:" << event->entity->ID();
    std::string data;
    auto *name = ComponentSerializeFactory::Instance().Serialize(
        event->component_base, &data);
    std::cout << " component:" << event->component_base->ClassIndex();
    if (!name)
    {
        std::cout << " name: ? data: ?";
    }
    else
    {
        std::cout << " name:" << name << " data:" << data;
    }
    std::cout << std::endl;
}

void SceneGridViewSystem::OnRemoveComponent(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const ComponentRemoveEvent *>(base);
    std::cout << "OnRemoveComponent entity:" << event->entity->ID();
    std::string data;
    auto *name = ComponentSerializeFactory::Instance().Serialize(
        event->component_base, &data);
    std::cout << " component:" << event->component_base->ClassIndex();
    if (!name)
    {
        std::cout << " name: ? data: ?";
    }
    else
    {
        std::cout << " name:" << name << " data:" << data;
    }
    std::cout << std::endl;
}

void SceneGridViewSystem::_Update(Scene *scene)
{
}

} // namespace cherry

