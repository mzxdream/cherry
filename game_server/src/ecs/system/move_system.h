#ifndef __CHERRY_MOVE_SYSTEM_H__
#define __CHERRY_MOVE_SYSTEM_H__

#include <mzx/ecs.h>

namespace cherry
{

class Scene;

class MoveSystem : public mzx::EntitySystem<MoveSystem, void(Scene *)>
{
private:
    virtual void _Update(Scene *scene) override;
};

} // namespace cherry

#endif
