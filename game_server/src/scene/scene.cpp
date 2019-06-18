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
    scene_entity_ = nullptr;
}

void Scene::Update()
{
    _Update();
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
