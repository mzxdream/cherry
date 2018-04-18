#ifndef __CHERRY_SCENE_MANAGER_H__
#define __CHERRY_SCENE_MANAGER_H__

#include <set>
#include <map>
#include <functional>
#include "scene/scene.h"

namespace cherry {

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();
    SceneManager(const SceneManager &) = delete;
    SceneManager & operator=(const SceneManager &) = delete;
public:
    void Update(int64_t delta_time);

    template <typename T, typename ...Args>
    T * CreateScene(SceneID id, Args && ...args)
    {
        static_assert(std::is_base_of<Scene, T>::value, "T must be extern Scene");
        SceneUUID uuid = ++next_scene_uuid_;
        T *scene = new T(id, uuid, std::forward<Args>(args)...);
        if (scene->Init())
        {
            delete scene;
            return nullptr;
        }
        return scene;
    }
    Scene * GetScene(SceneUUID uuid);
    void DestroyScene(SceneUUID uuid);
    void DelayDestroyScene(SceneUUID uuid);
    void ForeachScene(std::function<bool (Scene *)> cb);
private:
    std::map<SceneUUID, Scene *> scene_list_;
    std::set<SceneUUID> scene_destroy_list_;
    SceneUUID next_scene_uuid_;
};

}

#endif
