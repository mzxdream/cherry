#ifndef __CHERRY_WORLD_H__
#define __CHERRY_WORLD_H__

#include <mzx/ecs.h>
#include <mzx/simple_event.h>
#include <mzx/singleton.h>

#include <scene/scene_manager.h>

namespace cherry
{

class World : public mzx::Singleton<World>
{
public:
    friend class mzx::Singleton<World>;
    using EventManager = mzx::SimpleEventManager;

private:
    World();
    ~World();
    World(const World &) = delete;
    World &operator=(const World &) = delete;

public:
    EventManager &GetEventManager();
    SceneManager &GetSceneManager();

    bool Init();
    void Uninit();
    void Stop();
    void Run();

    int64_t CurTime() const;

private:
    SceneManager scene_manager_;
    EventManager event_manager_;
    int64_t cur_time_{0};
    volatile bool stop_flag_{false};
};

} // namespace cherry

#endif
