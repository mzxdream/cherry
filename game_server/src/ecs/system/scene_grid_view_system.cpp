#include <iostream>

#include <ecs/component/appear.h>
#include <ecs/component/location.h>
#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/single/scene_grid_view.h>
#include <ecs/component/view.h>
#include <ecs/event/component_event.h>
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
    scene_->GetSceneEntity()->AddComponent<SceneGridView>(100.0);
}

SceneGridViewSystem::~SceneGridViewSystem()
{
    scene_->GetSceneEntity()->RemoveComponent<SceneGridView>();
    scene_->GetEventManager().RemoveListener<ComponentAddEvent>(this);
    scene_->GetEventManager().RemoveListener<ComponentRemoveEvent>(this);
}

static void OnAddLocation(Scene *scene, mzx::Entity *entity,
                          mzx::ComponentBase *component_base)
{
    auto *scene_grid_view = scene->GetSceneEntity();
    auto *location =
        static_cast<mzx::Component<Location> *>(component_base)->Get();
    auto *appear = entity->GetComponent<Appear>();
    auto *view = entity->GetComponent<View>();
    if (!appear && !view)
    {
        return;
    }
    auto grid_x = static_cast<uint32_t>(location->position.X() /
                                        scene_grid_view->grid_size);
    auto grid_z = static_cast<uint32_t>(location->position.Z() /
                                        scene_grid_view->grid_size);
    uint32_t grid_index = grid_x << 16 + grid_z;
    scene_grid_view->grid_list[grid_index].insert(entity);
    scene_grid_view->entity_grid_index[entity] = grid_index;
    for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
    {
        for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
        {
            auto index = (i - 1) << 16 + (j - 1);
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
                auto *view = other->GetComponent<View>();
                if (!view)
                {
                    continue;
                }
                view->see_list.insert(entity);
                appear->follower_list.insert(other);
            }
        }
    }
}

static void OnAddAppear(Scene *scene, mzx::Entity *entity,
                        mzx::ComponentBase *component_base)
{
    auto *scene_grid_view = scene->GetSceneEntity();
    auto *location = entity->GetComponent<Location>();
    auto *appear = entity->GetComponent<Appear>();

    auto grid_x = static_cast<uint32_t>(location->position.X() /
                                        scene_grid_view->grid_size);
    auto grid_z = static_cast<uint32_t>(location->position.Z() /
                                        scene_grid_view->grid_size);
    uint32_t grid_index = grid_x << 16 + grid_z;
    scene_grid_view->grid_list[grid_index].insert(entity);
    scene_grid_view->entity_grid_index[entity] = grid_index;
    for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
    {
        for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
        {
            auto index = (i - 1) << 16 + (j - 1);
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
                auto *view = other->GetComponent<View>();
                if (!view)
                {
                    continue;
                }
                view->see_list.insert(entity);
                appear->follower_list.insert(other);
            }
        }
    }
}

static void OnRemoveAppear(Scene *scene, mzx::Entity *entity)
{
    auto *scene_grid_view = scene->GetSceneEntity();
    auto *location = entity->GetComponent<Location>();
    auto *appear = entity->GetComponent<Appear>();

    auto grid_x = static_cast<uint32_t>(location->position.X() /
                                        scene_grid_view->grid_size);
    auto grid_z = static_cast<uint32_t>(location->position.Z() /
                                        scene_grid_view->grid_size);
    uint32_t grid_index = grid_x << 16 + grid_z;
    scene_grid_view->grid_list[grid_index].insert(entity);
    scene_grid_view->entity_grid_index[entity] = grid_index;
    for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
    {
        for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
        {
            auto index = (i - 1) << 16 + (j - 1);
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
                auto *view = other->GetComponent<View>();
                if (!view)
                {
                    continue;
                }
                view->see_list.insert(entity);
                appear->follower_list.insert(other);
            }
        }
    }
}

static void OnAddView(Scene *scene, mzx::Entity *entity)
{
    auto *scene_grid_view = scene->GetSceneEntity();
    auto *location = entity->GetComponent<Location>();
    auto *view = entity->GetComponent<View>();

    auto grid_x = static_cast<uint32_t>(location->position.X() /
                                        scene_grid_view->grid_size);
    auto grid_z = static_cast<uint32_t>(location->position.Z() /
                                        scene_grid_view->grid_size);
    uint32_t grid_index = grid_x << 16 + grid_z;
    scene_grid_view->grid_list[grid_index].insert(entity);
    scene_grid_view->entity_grid_index[entity] = grid_index;
    for (uint32_t i = grid_x + 2; i >= grid_x && i > 0; --i)
    {
        for (uint32_t j = grid_z + 2; j >= grid_z && j > 0; --j)
        {
            auto index = (i - 1) << 16 + (j - 1);
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
                auto *appear = other->GetComponent<Appear>();
                if (!appear)
                {
                    continue;
                }
                view->see_list.insert(other);
                appear->follower_list.insert(entity);
            }
        }
    }
}

static void OnRemoveView(Scene *scene, mzx::Entity *entity)
{
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
    auto class_index = event->component_base->ClassIndex();
    if (class_index == mzx::Component<Location>::CLASS_INDEX)
    {
        OnAddLocation(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<Appear>::CLASS_INDEX)
    {
        OnAddAppear(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<View>::CLASS_INDEX)
    {
        OnAddView(scene_, event->entity, event->component_base);
    }
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
    auto class_index = event->component_base->ClassIndex();
    if (class_index == mzx::Component<Location>::CLASS_INDEX)
    {
        OnRemoveLocation(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<Appear>::CLASS_INDEX)
    {
        OnRemoveAppear(scene_, event->entity, event->component_base);
    }
    else if (class_index == mzx::Component<View>::CLASS_INDEX)
    {
        OnRemoveView(scene_, event->entity, event->component_base);
    }
}

void SceneGridViewSystem::_Update(Scene *scene)
{
}

} // namespace cherry

