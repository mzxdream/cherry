#ifndef __CHERRY_SCENE_GRID_VIEW_SYSTEM_H__
#define __CHERRY_SCENE_GRID_VIEW_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/simple_event.h>

namespace cherry
{

class Scene;

class SceneGridViewSystem
    : public mzx::EntitySystem<SceneGridViewSystem, void(Scene *)>
{
public:
    explicit SceneGridViewSystem(Scene *scene);
    virtual ~SceneGridViewSystem();

    void OnAddComponent(const mzx::SimpleEventBase *base);
    void OnRemoveComponent(const mzx::SimpleEventBase *base);
    void OnPositionChange(const mzx::SimpleEventBase *base);

private:
    void _Update(Scene *scene);

private:
    Scene *scene_{nullptr};
};

} // namespace cherry

#endif

