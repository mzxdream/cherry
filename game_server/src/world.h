#ifndef __CHERRY_WORLD_H__
#define __CHERRY_WORLD_H__

#include <mzx/singleton.h>
#include <mzx/noncopyable.h>
#include <mzx/ecs/entity_manager.h>
#include <mzx/event_manager.h>

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
    mzx::EventManager event_manager_;
};

}

#endif
