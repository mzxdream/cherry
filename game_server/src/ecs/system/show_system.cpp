#include <iostream>

#include <ecs/component/serialize/component_serialize.h>
#include <ecs/event/component_event.h>
#include <ecs/event/entity_event.h>
#include <ecs/system/show_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

ShowSystem::ShowSystem(Scene *scene)
    : scene_(scene)
{
    scene_->GetEventManager().AddListener<EntityAddEvent>(
        std::bind(&ShowSystem::OnAddEntity, this, std::placeholders::_1), this);
    scene_->GetEventManager().AddListener<EntityRemoveEvent>(
        std::bind(&ShowSystem::OnRemoveEntity, this, std::placeholders::_1),
        this);
    scene_->GetEventManager().AddListener<ComponentAddEvent>(
        std::bind(&ShowSystem::OnAddComponent, this, std::placeholders::_1),
        this);
    scene_->GetEventManager().AddListener<ComponentRemoveEvent>(
        std::bind(&ShowSystem::OnRemoveComponent, this, std::placeholders::_1),
        this);
}

ShowSystem::~ShowSystem()
{
    scene_->GetEventManager().RemoveListener<EntityAddEvent>(this);
    scene_->GetEventManager().RemoveListener<EntityRemoveEvent>(this);
    scene_->GetEventManager().RemoveListener<ComponentAddEvent>(this);
    scene_->GetEventManager().RemoveListener<ComponentRemoveEvent>(this);
}

void ShowSystem::OnAddEntity(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const EntityAddEvent *>(base);
    std::cout << "OnAddEntity entity:" << event->entity->ID() << std::endl;
}

void ShowSystem::OnRemoveEntity(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const EntityRemoveEvent *>(base);
    std::cout << "OnRemoveEntity entity:" << event->entity->ID() << std::endl;
}

void ShowSystem::OnAddComponent(const mzx::SimpleEventBase *base)
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

void ShowSystem::OnRemoveComponent(const mzx::SimpleEventBase *base)
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

void ShowSystem::_Update(Scene *scene)
{
}

} // namespace cherry

