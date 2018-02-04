#ifndef __CHERRY_WORLD_H__
#define __CHERRY_WORLD_H__

#include <mzx/singleton.h>
#include <mzx/noncopyable.h>
#include <mzx/ecs/entity_manager.h>
#include <mzx/ecs/event_manager.h>
#include <mzx/ecs/system_manager.h>

namespace cherry {

class World
    : public mzx::Singleton<World>
    , public mzx::Noncopyable
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
private:
    bool stop_flag_;
    mzx::ecs::EntityManager entity_manager_;
    mzx::ecs::EventManager event_manager_;
    mzx::ecs::SystemManager system_manager_;
};

}

#endif
