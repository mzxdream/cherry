#include <iostream>

#include <ecs/component/location.h>
#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/single/scene_grid_view.h>
#include <ecs/component/view.h>
#include <ecs/event/component_event.h>
#include <ecs/event/position_change_event.h>
#include <ecs/system/scene_grid_view_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

SceneGridViewSystem::SceneGridViewSystem(Scene *scene)
    : scene_(scene)
{
    scene_->GetEventManager().AddListener<ComponentAddEvent>(
        std::bind(&SceneGridViewSystem::OnAddComponent, this,
                  std::placeholders::_1),
        this);
    scene_->GetEventManager().AddListener<ComponentRemoveEvent>(
        std::bind(&SceneGridViewSystem::OnRemoveComponent, this,
                  std::placeholders::_1),
        this);
    scene_->GetEventManager().AddListener<PositionChangeEvent>(
        std::bind(&SceneGridViewSystem::OnPositionChange, this,
                  std::placeholders::_1),
        this);
    scene_->GetSceneEntity()->AddComponent<SceneGridView>(100.0);
}

SceneGridViewSystem::~SceneGridViewSystem()
{
    scene_->GetSceneEntity()->RemoveComponent<SceneGridView>();
    scene_->GetEventManager().RemoveListener<ComponentAddEvent>(this);
    scene_->GetEventManager().RemoveListener<ComponentRemoveEvent>(this);
    scene_->GetEventManager().RemoveListener<PositionChangeEvent>(this);
}

static void OnAddLocation(Scene *scene, mzx::Entity *entity,
                          mzx::ComponentBase *component_base)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    if (entity->HasComponent<ViewPublish>())
    {
        scene_grid_view->publish_add_list.insert(entity);
    }
    if (entity->HasComponent<ViewSubscribe>())
    {
        scene_grid_view->subscribe_add_list.insert(entity);
    }
}

static void OnAddViewPublish(Scene *scene, mzx::Entity *entity,
                             mzx::ComponentBase *component_base)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    if (entity->HasComponent<Location>())
    {
        scene_grid_view->publish_add_list.insert(entity);
    }
}

static void OnAddViewSubscribe(Scene *scene, mzx::Entity *entity,
                               mzx::ComponentBase *component_base)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    if (entity->HasComponent<Location>())
    {
        scene_grid_view->subscribe_add_list.insert(entity);
    }
}

static void OnRemoveLocation(Scene *scene, mzx::Entity *entity,
                             mzx::ComponentBase *component_base)
{
    if (!entity->HasComponent<ViewPublish>() ||
        !entity->HasComponent<ViewSubscribe>())
    {
        return;
    }
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    auto iter_index = scene_grid_view->entity_grid_index.find(entity);
    if (iter_index != scene_grid_view->entity_grid_index.end())
    {
        scene_grid_view->grid_list[iter_index->second].erase(entity);
        scene_grid_view->entity_grid_index.erase(iter_index);
    }
    scene_grid_view->publish_add_list.erase(entity);
    scene_grid_view->subscribe_add_list.erase(entity);
    scene_grid_view->position_change_list.erase(entity);
    auto *view_publish = entity->GetComponent<ViewPublish>();
    if (view_publish)
    {
        for (auto &subscribe : view_publish->subscribe_list)
        {
            subscribe->GetComponent<ViewSubscribe>()->publish_list.erase(
                entity);
        }
    }
    auto *view_subscribe = entity->GetComponent<ViewSubscribe>();
    if (view_subscribe)
    {
        for (auto &publish : view_subscribe->publish_list)
        {
            publish->GetComponent<ViewPublish>()->subscribe_list.erase(entity);
        }
    }
}

static void OnRemoveViewPublish(Scene *scene, mzx::Entity *entity,
                                mzx::ComponentBase *component_base)
{
    if (!entity->HasComponent<Location>())
    {
        return;
    }
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    if (!entity->HasComponent<ViewSubscribe>())
    {
        auto iter_index = scene_grid_view->entity_grid_index.find(entity);
        if (iter_index != scene_grid_view->entity_grid_index.end())
        {
            scene_grid_view->grid_list[iter_index->second].erase(entity);
            scene_grid_view->entity_grid_index.erase(iter_index);
        }
        scene_grid_view->position_change_list.erase(entity);
    }
    scene_grid_view->publish_add_list.erase(entity);
    auto *view_publish =
        static_cast<mzx::Component<ViewPublish> *>(component_base)->Get();
    if (view_publish)
    {
        for (auto &subscribe : view_publish->subscribe_list)
        {
            subscribe->GetComponent<ViewSubscribe>()->publish_list.erase(
                entity);
        }
    }
}

static void OnRemoveViewSubscribe(Scene *scene, mzx::Entity *entity,
                                  mzx::ComponentBase *component_base)
{
    if (!entity->HasComponent<Location>())
    {
        return;
    }
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    if (!entity->HasComponent<ViewPublish>())
    {
        auto iter_index = scene_grid_view->entity_grid_index.find(entity);
        if (iter_index != scene_grid_view->entity_grid_index.end())
        {
            scene_grid_view->grid_list[iter_index->second].erase(entity);
            scene_grid_view->entity_grid_index.erase(iter_index);
        }
        scene_grid_view->position_change_list.erase(entity);
    }
    scene_grid_view->subscribe_add_list.erase(entity);
    auto *view_subscribe =
        static_cast<mzx::Component<ViewSubscribe> *>(component_base)->Get();
    if (view_subscribe)
    {
        for (auto &publish : view_subscribe->publish_list)
        {
            publish->GetComponent<ViewPublish>()->subscribe_list.erase(entity);
        }
    }
}

void SceneGridViewSystem::OnAddComponent(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const ComponentAddEvent *>(base);
    auto class_index = event->component_base->ClassIndex();
    if (class_index == mzx::Component<Location>::CLASS_INDEX)
    {
        OnAddLocation(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<ViewPublish>::CLASS_INDEX)
    {
        OnAddViewPublish(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<ViewSubscribe>::CLASS_INDEX)
    {
        OnAddViewSubscribe(scene_, event->entity, event->component_base);
    }
}

void SceneGridViewSystem::OnRemoveComponent(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const ComponentRemoveEvent *>(base);
    auto class_index = event->component_base->ClassIndex();
    if (class_index == mzx::Component<Location>::CLASS_INDEX)
    {
        OnRemoveLocation(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<ViewPublish>::CLASS_INDEX)
    {
        OnRemoveViewPublish(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<ViewSubscribe>::CLASS_INDEX)
    {
        OnRemoveViewSubscribe(scene_, event->entity, event->component_base);
    }
}

void SceneGridViewSystem::OnPositionChange(const mzx::SimpleEventBase *base)
{
    auto *event = static_cast<const PositionChangeEvent *>(base);
    auto *entity = event->entity;
    if (entity->HasComponent<ViewPublish>() ||
        entity->HasComponent<ViewSubscribe>())
    {
        auto *scene_grid_view =
            scene_->GetSceneEntity()->GetComponent<SceneGridView>();
        scene_grid_view->position_change_list.insert(entity);
    }
}

static void HandlePublishAdd(Scene *scene)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    for (auto &entity : scene_grid_view->publish_add_list)
    {
        uint32_t grid_x = 0;
        uint32_t grid_z = 0;
        auto iter_grid = scene_grid_view->entity_grid_index.find(entity);
        if (iter_grid == scene_grid_view->entity_grid_index.end())
        {
            auto *location = entity->GetComponent<Location>();
            grid_x = static_cast<uint32_t>(location->position.X() /
                                           scene_grid_view->grid_size);
            grid_z = static_cast<uint32_t>(location->position.Z() /
                                           scene_grid_view->grid_size);
            uint32_t grid_index = (grid_x << 16) + grid_z;
            scene_grid_view->grid_list[grid_index].insert(entity);
            scene_grid_view->entity_grid_index[entity] = grid_index;
        }
        else
        {
            grid_x = iter_grid->second >> 16;
            grid_z = iter_grid->second & 0xFFFF;
        }
        auto *view_publish = entity->GetComponent<ViewPublish>();
        for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
        {
            for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
            {
                auto index = ((i - 1) << 16) + (j - 1);
                auto iter_grid = scene_grid_view->grid_list.find(index);
                if (iter_grid == scene_grid_view->grid_list.end())
                {
                    continue;
                }
                for (auto &other : iter_grid->second)
                {
                    if (other == entity)
                    {
                        continue;
                    }
                    auto *other_view_subscribe =
                        other->GetComponent<ViewSubscribe>();
                    if (other_view_subscribe)
                    {
                        other_view_subscribe->publish_list.insert(entity);
                        view_publish->subscribe_list.insert(other);
                    }
                }
            }
        }
    }
    scene_grid_view->publish_add_list.clear();
}

static void HandleSubscribeAdd(Scene *scene)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    for (auto &entity : scene_grid_view->subscribe_add_list)
    {
        uint32_t grid_x = 0;
        uint32_t grid_z = 0;
        auto iter_grid = scene_grid_view->entity_grid_index.find(entity);
        if (iter_grid == scene_grid_view->entity_grid_index.end())
        {
            auto *location = entity->GetComponent<Location>();
            grid_x = static_cast<uint32_t>(location->position.X() /
                                           scene_grid_view->grid_size);
            grid_z = static_cast<uint32_t>(location->position.Z() /
                                           scene_grid_view->grid_size);
            uint32_t grid_index = (grid_x << 16) + grid_z;
            scene_grid_view->grid_list[grid_index].insert(entity);
            scene_grid_view->entity_grid_index[entity] = grid_index;
        }
        else
        {
            grid_x = iter_grid->second >> 16;
            grid_z = iter_grid->second & 0xFFFF;
        }
        auto *view_subscribe = entity->GetComponent<ViewSubscribe>();
        for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
        {
            for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
            {
                auto index = ((i - 1) << 16) + (j - 1);
                auto iter_grid = scene_grid_view->grid_list.find(index);
                if (iter_grid == scene_grid_view->grid_list.end())
                {
                    continue;
                }
                for (auto &other : iter_grid->second)
                {
                    if (other == entity)
                    {
                        continue;
                    }
                    auto *other_view_publish =
                        other->GetComponent<ViewPublish>();
                    if (other_view_publish)
                    {
                        other_view_publish->subscribe_list.insert(entity);
                        view_subscribe->publish_list.insert(other);
                    }
                }
            }
        }
    }
    scene_grid_view->subscribe_add_list.clear();
}

static bool IsInGrid(uint32_t x, uint32_t z, uint32_t grid_x, uint32_t grid_z)
{
    if (x > grid_x + 1 || x + 1 < grid_x)
    {
        return false;
    }
    if (z > grid_z + 1 || z + 1 < grid_z)
    {
        return false;
    }
    return true;
}

static void HandlePositionChange(Scene *scene)
{
    auto *scene_grid_view =
        scene->GetSceneEntity()->GetComponent<SceneGridView>();
    for (auto &entity : scene_grid_view->position_change_list)
    {
        auto iter_grid = scene_grid_view->entity_grid_index.find(entity);
        if (iter_grid == scene_grid_view->entity_grid_index.end())
        {
            continue;
        }
        auto old_grid_index = iter_grid->second;
        auto *location = entity->GetComponent<Location>();
        auto grid_x = static_cast<uint32_t>(location->position.X() /
                                            scene_grid_view->grid_size);
        auto grid_z = static_cast<uint32_t>(location->position.Z() /
                                            scene_grid_view->grid_size);
        uint32_t grid_index = (grid_x << 16) + grid_z;
        if (old_grid_index == grid_index)
        {
            continue;
        }
        auto old_grid_x = old_grid_index >> 16;
        auto old_grid_z = old_grid_index & 0xFFFF;
        scene_grid_view->grid_list[old_grid_index].erase(entity);
        scene_grid_view->grid_list[grid_index].insert(entity);
        scene_grid_view->entity_grid_index[entity] = grid_index;

        auto *view_publish = entity->GetComponent<ViewPublish>();
        auto *view_subscribe = entity->GetComponent<ViewSubscribe>();
        for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
        {
            for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
            {
                if (IsInGrid(i - 1, j - 1, old_grid_x, old_grid_z))
                {
                    continue;
                }
                auto index = ((i - 1) << 16) + (j - 1);
                auto iter_grid = scene_grid_view->grid_list.find(index);
                if (iter_grid == scene_grid_view->grid_list.end())
                {
                    continue;
                }
                for (auto &other : iter_grid->second)
                {
                    if (other == entity)
                    {
                        continue;
                    }
                    if (view_publish)
                    {
                        auto *other_view_subscribe =
                            other->GetComponent<ViewSubscribe>();
                        if (other_view_subscribe)
                        {
                            other_view_subscribe->publish_list.insert(entity);
                            view_publish->subscribe_list.insert(other);
                        }
                    }
                    if (view_subscribe)
                    {
                        auto *other_view_publish =
                            other->GetComponent<ViewPublish>();
                        if (other_view_publish)
                        {
                            other_view_publish->subscribe_list.insert(entity);
                            view_subscribe->publish_list.insert(other);
                        }
                    }
                }
            }
        }
        for (uint32_t i = old_grid_x + 2; i >= old_grid_x && i > 0; --i)
        {
            for (uint32_t j = old_grid_z + 2; j >= old_grid_z && j > 0; --j)
            {
                if (IsInGrid(i - 1, j - 1, grid_x, grid_z))
                {
                    continue;
                }
                auto index = ((i - 1) << 16) + (j - 1);
                auto iter_grid = scene_grid_view->grid_list.find(index);
                if (iter_grid == scene_grid_view->grid_list.end())
                {
                    continue;
                }
                for (auto &other : iter_grid->second)
                {
                    if (other == entity)
                    {
                        continue;
                    }
                    if (view_publish)
                    {
                        auto *other_view_subscribe =
                            other->GetComponent<ViewSubscribe>();
                        if (other_view_subscribe)
                        {
                            other_view_subscribe->publish_list.erase(entity);
                            view_publish->subscribe_list.erase(other);
                        }
                    }
                    if (view_subscribe)
                    {
                        auto *other_view_publish =
                            other->GetComponent<ViewPublish>();
                        if (other_view_publish)
                        {
                            other_view_publish->subscribe_list.erase(entity);
                            view_subscribe->publish_list.erase(other);
                        }
                    }
                }
            }
        }
    }
    scene_grid_view->position_change_list.clear();
}

void SceneGridViewSystem::_Update(Scene *scene)
{
    HandlePublishAdd(scene);
    HandleSubscribeAdd(scene);
    HandlePositionChange(scene);
}

} // namespace cherry

