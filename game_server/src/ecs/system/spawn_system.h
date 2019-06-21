#ifndef __CHERRY_SPAWN_SYSTEM_H__
#define __CHERRY_SPAWN_SYSTEM_H__

#include <mzx/ecs.h>

namespace cherry
{

class Scene;

class SpawnSystem : public mzx::EntitySystem<SpawnSystem, void(Scene *)>
{
private:
    virtual void _Update(Scene *scene) override;
};

} // namespace cherry

#endif
