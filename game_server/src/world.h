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
    friend class mzx::Singleton<World>;
private:
    World();
    ~World();
public:
    bool Init();
    void Uninit();
    void Stop();
    void Run();

    int64_t CurTime() const;
private:
    bool stop_flag_;
    int64_t cur_time_;

    mzx::EntityManager entity_manager_;
    mzx::EntitySystemManager system_manager_;
    mzx::EventManager<EventType, void (World *, const Event *)> event_manager_;
};

}

#endif
