#ifndef __CHERRY_SPAWN_SYSTEM_H__
#define __CHERRY_SPAWN_SYSTEM_H__

#include <mzx/ecs.h>

namespace cherry
{

class Scene;

class SpawnSystem : public mzx::EntitySystem<SpawnSystem>
{
public:
    SpawnSystem();
    virtual ~SpawnSystem();
    SpawnSystem(const SpawnSystem &) = delete;
    SpawnSystem &operator=(const SpawnSystem &) = delete;

private:
    virtual bool _Init() override;
    virtual void _Uninit() override;

public:
    void Update(Scene *scene);
};

} // namespace cherry

#endif
