#ifndef __CHERRY_WORLD_H__
#define __CHERRY_WORLD_H__

#include <mzx/singleton.h>
#include <mzx/ecs.h>
#include <mzx/event.h>

#include "ecs/event/event.h"
#include "ecs/event/event_def.h"

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
    bool Init();
    void Uninit();
    void Stop();
    void Run();

    int64_t CurTime() const;
    EventManager & GetEventManager();
private:
    bool stop_flag_;
    int64_t cur_time_;

    mzx::EntityManager entity_manager_;
    mzx::EntitySystemManager system_manager_;
    EventManager event_manager_;
};

}

#endif
