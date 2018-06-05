#ifndef __CHERRY_SPAWN_SYSTEM_H__
#define __CHERRY_SPAWN_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/event.h>

namespace cherry {

class Event;
class Scene;

class SpawnSystem
    : public mzx::EntitySystem<SpawnSystem>
{
public:
    explicit SpawnSystem(Scene *scene);
    virtual ~SpawnSystem();
    SpawnSystem(const SpawnSystem &) = delete;
    SpawnSystem & operator=(const SpawnSystem &) = delete;
private:
    virtual bool _Init() override;
    virtual void _Uninit() override;
    virtual void _Update(int64_t now_time) override;
    virtual bool _Configure() override;
    virtual void _Unconfigure() override;
private:
    Scene *scene_;
};

}

#endif
