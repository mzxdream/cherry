#include <scene/scene.h>

namespace cherry
{

Scene::Scene(SceneUUID uuid)
    : uuid_(uuid)
    , need_destroy_(false)
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

Scene::EventManager &Scene::GetEventManager()
{
    return event_manager_;
}

mzx::EntityManager &Scene::GetEntityManager()
{
    return entity_manager_;
}

mzx::EntitySystemManager &Scene::GetSystemManager()
{
    return system_manager_;
}

bool Scene::Init()
{
    return _Init();
}

void Scene::Uninit()
{
    _Uninit();
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
