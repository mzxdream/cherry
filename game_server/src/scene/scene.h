#ifndef __CHERRY_SCENE_H__
#define __CHERRY_SCENE_H__

#include <cstdint>
#include <mzx/ecs.h>
#include <mzx/event.h>

#include "ecs/event/event.h"
#include "ecs/event/event_def.h"

namespace cherry
{

using SceneID = int64_t;
constexpr SceneID SCENE_ID_INVALID = (SceneID)-1;

using SceneUUID = int64_t;
constexpr SceneUUID SCENE_UUID_INVALID = (SceneUUID)-1;

class Scene
{
public:
    using EventManager = mzx::EventManager<EventType, void(const Event *)>;

public:
    Scene(SceneID id, SceneUUID uuid);
    virtual ~Scene() = 0;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

public:
    SceneID Id() const;
    SceneUUID Uuid() const;
    void SetNeedDestroy(bool need_destroy = true);
    bool IsNeedDestroy() const;
    EventManager &GetEventManager();
    mzx::EntityManager &GetEntityManager();
    mzx::EntitySystemManager &GetSystemManager();

    bool Init();
    void Uninit();
    void Update(int64_t cur_time);

private:
    virtual bool _Init();
    virtual void _Uninit();
    virtual void _Update(int64_t cur_time);

private:
    SceneID id_{SCENE_ID_INVALID};
    SceneUUID uuid_{SCENE_UUID_INVALID};
    bool need_destroy_{false};
    EventManager event_manager_;
    mzx::EntityManager entity_manager_;
    mzx::EntitySystemManager system_manager_;
};

} // namespace cherry

#endif
