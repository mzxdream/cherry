#ifndef __CHERRY_SCENE_MANAGER_H__
#define __CHERRY_SCENE_MANAGER_H__

#include <functional>
#include <map>
#include <set>

#include <scene/scene.h>

namespace cherry
{

class SceneManager final
{
public:
    SceneManager();
    ~SceneManager();
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

public:
    std::size_t SceneCount() const;

    template <typename T, typename... Args>
    T *CreateScene(Args &&... args)
    {
        static_assert(std::is_base_of<Scene, T>::value,
                      "T must be extern Scene");
        auto *scene = new T(++next_scene_uuid_, std::forward<Args>(args)...);
        if (!scene->Init())
        {
            delete scene;
            return nullptr;
        }
        scene_list_.emplace(scene->UUID(), scene);
        return scene;
    }
    Scene *GetScene(SceneUUID uuid);
    void DestroyScene(SceneUUID uuid);
    void DelayDestroyScene(SceneUUID uuid);
    void ForeachScene(std::function<bool(Scene *)> cb);
    void Update();

private:
    std::map<SceneUUID, Scene *> scene_list_;
    SceneUUID next_scene_uuid_;
};

} // namespace cherry

#endif
