#include "scene/scene.h"

namespace cherry {

Scene::Scene(SceneID id, SceneUUID uuid)
    : id_(id)
    , uuid_(uuid)
    , need_destroy_(false)
{
}

Scene::~Scene()
{
}

SceneID Scene::Id() const
{
    return id_;
}

SceneUUID Scene::Uuid() const
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

Scene::EventManager & Scene::GetEventManager()
{
    return event_manager_;
}

mzx::EntityManager & Scene::GetEntityManager()
{
    return entity_manager_;
}

mzx::EntitySystemManager & Scene::GetSystemManager()
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

void Scene::Update(int64_t delta_time)
{
    _Update(delta_time);
}

bool Scene::_Init()
{
    return true;
}

void Scene::_Uninit()
{

}

void Scene::_Update(int64_t delta_time)
{

}

}
