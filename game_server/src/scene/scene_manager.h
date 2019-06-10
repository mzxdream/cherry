#ifndef __CHERRY_SCENE_MANAGER_H__
#define __CHERRY_SCENE_MANAGER_H__

#include <functional>
#include <map>
#include <set>

#include "scene/scene.h"

namespace cherry
{

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

public:
    template <typename T, typename... Args>
    T *CreateScene(SceneID id, Args &&... args)
    {
        static_assert(std::is_base_of<Scene, T>::value,
                      "T must be extern Scene");
        T *scene = new T(id, ++next_scene_uuid_, std::forward<Args>(args)...);
        if (!scene->Init())
        {
            scene->Uninit();
            delete scene;
            return nullptr;
        }
        return scene;
    }
    Scene *GetScene(SceneUUID uuid);
    void DestroyScene(SceneUUID uuid);
    void DelayDestroyScene(SceneUUID uuid);
    void ForeachScene(std::function<bool(Scene *)> cb);
    void Update(int64_t delta_time);

private:
    std::map<SceneUUID, Scene *> scene_list_;
    SceneUUID next_scene_uuid_;
};

} // namespace cherry

#endif
