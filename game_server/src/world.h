#ifndef __CHERRY_WORLD_H__
#define __CHERRY_WORLD_H__

#include <mzx/singleton.h>
#include <mzx/ecs.h>
#include <mzx/event.h>

#include "ecs/event/event.h"
#include "ecs/event/event_def.h"
#include "scene/scene_manager.h"

namespace cherry {

class World
    : public mzx::Singleton<World>
{
public:
    friend class mzx::Singleton<World>;
    using EventManager = mzx::EventManager<EventType, void (const Event *)>;
private:
    World();
    ~World();
    World(const World &) = delete;
    World & operator=(const World &) = delete;
public:
    EventManager & GetEventManager();

    bool Init();
    void Uninit();
    void Stop();
    void Run();

    int64_t CurTime() const;
private:
    bool stop_flag_;
    int64_t cur_time_;

    EventManager event_manager_;
    SceneManager scene_manager_;
};

}

#endif
