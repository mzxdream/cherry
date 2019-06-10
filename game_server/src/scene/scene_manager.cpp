#include <scene/scene_manager.h>

namespace cherry
{

SceneManager::SceneManager()
    : next_scene_uuid_(0)
{
}

SceneManager::~SceneManager()
{
    for (auto &iter_scene : scene_list_)
    {
        iter_scene.second->Uninit();
        delete iter_scene.second;
    }
    scene_list_.clear();
    next_scene_uuid_ = 0;
}

Scene *SceneManager::GetScene(SceneUUID uuid)
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
    iter_scene->second->Uninit();
    delete iter_scene->second;
    scene_list_.erase(iter_scene);
}

void SceneManager::DelayDestroyScene(SceneUUID uuid)
{
    auto iter_scene = scene_list_.find(uuid);
    if (iter_scene == scene_list_.end())
    {
        return;
    }
    iter_scene->second->SetNeedDestroy();
}

void SceneManager::ForeachScene(std::function<bool(Scene *)> cb)
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

void SceneManager::Update()
{
    auto iter_scene = scene_list_.begin();
    while (iter_scene != scene_list_.end())
    {
        if (iter_scene->second->IsNeedDestroy())
        {
            iter_scene->second->Uninit();
            delete iter_scene->second;
            iter_scene = scene_list_.erase(iter_scene);
        }
        else
        {
            iter_scene->second->Update();
            ++iter_scene;
        }
    }
}

} // namespace cherry
