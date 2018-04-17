#include "scene/scene_manager.h"

namespace cherry {

SceneManager::SceneManager()
    : next_scene_uuid_(0)
{

}

SceneManager::~SceneManager()
{
    Uninit();
}

bool SceneManager::Init()
{
    return true;
}

void SceneManager::Uninit()
{
    auto iter_scene = scene_list_.begin();
    while (iter_scene != scene_list_.end())
    {
        delete iter_scene->second;
        iter_scene = scene_list_.erase(iter_scene);
    }
    scene_destroy_list_.clear();
    next_scene_uuid_ = 0;
}

void SceneManager::Update(int64_t delta_time)
{
    for (auto &iter_scene : scene_list_)
    {
        iter_scene.second->Update(delta_time);
    }
    auto iter_destroy_scene = scene_destroy_list_.begin();
    while (iter_destroy_scene != scene_destroy_list_.end())
    {
        DestroyScene(*iter_destroy_scene);
        iter_destroy_scene = scene_destroy_list_.erase(iter_destroy_scene);
    }
}

Scene * SceneManager::GetScene(SceneUUID uuid)
{
    auto iter_scene = scene_list_.find(uuid);
    if (iter_scene == scene_list_.end())
    {
        return nullptr;
    }
    return iter_scene->second;
}

void SceneManager::DestroyScene(SceneUUID uuid)
{
    auto iter_scene = scene_list_.find(uuid);
    if (iter_scene == scene_list_.end())
    {
        return;
    }
    delete iter_scene->second;
    scene_list_.erase(iter_scene);
}

void SceneManager::DelayDestroyScene(SceneUUID uuid)
{
    scene_destroy_list_.insert(uuid);
}

void SceneManager::ForeachScene(std::function<bool (Scene *)> cb)
{
    if (!cb)
    {
        return;
    }
    for (auto &iter_scene : scene_list_)
    {
        if (!cb(iter_scene.second))
        {
            break;
        }
    }
}

}
