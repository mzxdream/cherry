#include <iostream>

#include <ecs/component/hatred.h>
#include <ecs/component/location.h>
#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/view.h>
#include <ecs/event/component_event.h>
#include <ecs/system/hatred_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

HatredSystem::HatredSystem(Scene *scene)
    : scene_(scene)
{
    scene_->GetEventManager().AddListener<ComponentRemoveEvent>(
        std::bind(&HatredSystem::OnRemoveComponent, this,
                  std::placeholders::_1),
        this);
}

HatredSystem::~HatredSystem()
{
    scene_->GetEventManager().RemoveListener<ComponentRemoveEvent>(this);
}

static void OnRemoveHatredPublish(Scene *scene, mzx::Entity *entity,
                                  mzx::ComponentBase *component_base)
{
    auto *hatred_publish =
        static_cast<mzx::Component<HatredPublish> *>(component_base)->Get();
    for (auto &subscribe : hatred_publish->subscribe_list)
    {
        auto *other_hatred_subscribe =
            subscribe->GetComponent<HatredSubscribe>();
        auto iter_index = other_hatred_subscribe->entity_index.find(entity);
        if (iter_index != other_hatred_subscribe->entity_index.end())
        {
            other_hatred_subscribe->publish_list.erase(iter_index->second);
            other_hatred_subscribe->entity_index.erase(iter_index);
        }
    }
}

static void OnRemoveHatredSubscribe(Scene *scene, mzx::Entity *entity,
                                    mzx::ComponentBase *component_base)
{
    auto *hatred_subscribe =
        static_cast<mzx::Component<HatredSubscribe> *>(component_base)->Get();
    for (auto &iter_publish : hatred_subscribe->entity_index)
    {
        iter_publish.first->GetComponent<HatredPublish>()->subscribe_list.erase(
            entity);
    }
}

void HatredSystem::OnRemoveComponent(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const ComponentRemoveEvent *>(base);
    auto class_index = event->component_base->ClassIndex();
    if (class_index == mzx::Component<HatredPublish>::CLASS_INDEX)
    {
        OnRemoveHatredPublish(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<HatredSubscribe>::CLASS_INDEX)
    {
        OnRemoveHatredSubscribe(scene_, event->entity, event->component_base);
    }
}

void HatredSystem::_Update(Scene *scene)
{
    scene->GetEntityManager()
        .ForeachEntity<HatredSubscribe, HatredDistanceCheck, ViewSubscribe,
                       Location>([](mzx::Entity *entity) {
            auto *hatred_subscribe = entity->GetComponent<HatredSubscribe>();
            auto *hatred_distance_check =
                entity->GetComponent<HatredDistanceCheck>();
            auto *view_subscribe = entity->GetComponent<ViewSubscribe>();
            auto *location = entity->GetComponent<Location>();
            for (auto &other : view_subscribe->publish_list)
            {
                auto *other_location = other->GetComponent<Location>();
                auto *other_hatred_publish =
                    other->GetComponent<HatredPublish>();
                if (!other_location || !other_hatred_publish)
                {
                    continue;
                }
                if (mzx::Vector3<double>::Distance(location->position,
                                                   other_location->position) <=
                    hatred_distance_check->distance)
                {
                    if (hatred_subscribe->entity_index.find(other) ==
                        hatred_subscribe->entity_index.end())
                    {
                        auto result = hatred_subscribe->publish_list.insert(
                            HatredSubscribe::SortType::value_type(1, other));
                        hatred_subscribe->entity_index[other] = result;
                        other_hatred_publish->subscribe_list.insert(entity);
                    }
                }
            }
            return true;
        });
}

} // namespace cherry

