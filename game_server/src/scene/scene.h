#ifndef __CHERRY_SCENE_H__
#define __CHERRY_SCENE_H__

#include <cstdint>
#include <mzx/ecs.h>
#include <mzx/simple_event.h>

namespace cherry
{

using SceneUUID = int64_t;
constexpr SceneUUID SCENE_UUID_INVALID = (SceneUUID)-1;

class Scene
{
public:
    using EventManager = mzx::SimpleEventManager;
    using EntityManager = mzx::EntityManager;
    using EntitySystemManager = mzx::EntitySystemManager<void(Scene *)>;

public:
    Scene(SceneUUID uuid);
    virtual ~Scene() = 0;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

public:
    SceneUUID UUID() const;
    void SetNeedDestroy(bool need_destroy = true);
    bool IsNeedDestroy() const;
    mzx::Entity *GetSceneEntity();
    EventManager &GetEventManager();
    EntityManager &GetEntityManager();
    EntitySystemManager &GetSystemManager();

    bool Init();
    void Uninit();
    void Update();

    void OnAddComponent(mzx::Entity *entity, mzx::ComponentBase *base);
    void OnRemoveComponent(mzx::Entity *entity, mzx::ComponentBase *base);
    void OnAddEntity(mzx::Entity *entity);
    void OnRemoveEntity(mzx::Entity *entity);

private:
    virtual bool _Init();
    virtual void _Uninit();
    virtual void _Update();

private:
    SceneUUID uuid_{SCENE_UUID_INVALID};
    bool need_destroy_{false};
    mzx::Entity *scene_entity_{nullptr};
    EventManager event_manager_;
    EntityManager entity_manager_;
    EntitySystemManager system_manager_;
};

} // namespace cherry

#endif
