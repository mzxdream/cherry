#include <ecs/event/component_event.h>
#include <ecs/event/entity_event.h>
#include <scene/scene.h>

namespace cherry
{

Scene::Scene(SceneUUID uuid)
    : uuid_(uuid)
{
}

Scene::~Scene()
{
}

SceneUUID Scene::UUID() const
{
    return uuid_;
}

void Scene::SetNeedDestroy(bool need_destroy)
{
    need_destroy_ = need_destroy;
}

bool Scene::IsNeedDestroy() const
{
    return need_destroy_;
}

mzx::Entity &Scene::GetSceneEntity()
{
    return *scene_entity_;
}

Scene::EventManager &Scene::GetEventManager()
{
    return event_manager_;
}

Scene::EntityManager &Scene::GetEntityManager()
{
    return entity_manager_;
}

Scene::EntitySystemManager &Scene::GetSystemManager()
{
    return system_manager_;
}

bool Scene::Init()
{
    entity_manager_.ComponentAddEvent().AddListener(
        std::bind(&Scene::OnAddComponent, this, std::placeholders::_1,
                  std::placeholders::_2),
        this);
    entity_manager_.ComponentRemoveEvent().AddListener(
        std::bind(&Scene::OnRemoveComponent, this, std::placeholders::_1,
                  std::placeholders::_2),
        this);
    entity_manager_.EntityAddEvent().AddListener(
        std::bind(&Scene::OnAddEntity, this, std::placeholders::_1), this);
    entity_manager_.EntityRemoveEvent().AddListener(
        std::bind(&Scene::OnRemoveEntity, this, std::placeholders::_1), this);

    scene_entity_ = entity_manager_.AddEntity();
    if (!scene_entity_)
    {
        return false;
    }
    return _Init();
}

void Scene::Uninit()
{
    _Uninit();
    system_manager_.RemoveAllSystem();
    entity_manager_.RemoveAllEntity();
    event_manager_.RemoveAllEvent();
    scene_entity_ = nullptr;

    entity_manager_.ComponentAddEvent().RemoveListener(this);
    entity_manager_.ComponentRemoveEvent().RemoveListener(this);
    entity_manager_.EntityAddEvent().RemoveListener(this);
    entity_manager_.EntityRemoveEvent().RemoveListener(this);
}

void Scene::Update()
{
    _Update();
}

void Scene::OnAddComponent(mzx::Entity *entity, mzx::ComponentBase *base)
{
    event_manager_.InvokeEmplace<ComponentAddEvent>(entity, base);
}

void Scene::OnRemoveComponent(mzx::Entity *entity, mzx::ComponentBase *base)
{
    event_manager_.InvokeEmplace<ComponentRemoveEvent>(entity, base);
}

void Scene::OnAddEntity(mzx::Entity *entity)
{
    event_manager_.InvokeEmplace<EntityAddEvent>(entity);
}

void Scene::OnRemoveEntity(mzx::Entity *entity)
{
    event_manager_.InvokeEmplace<EntityRemoveEvent>(entity);
}

bool Scene::_Init()
{
    return true;
}

void Scene::_Uninit()
{
}

void Scene::_Update()
{
}

} // namespace cherry
