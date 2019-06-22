#ifndef __CHERRY_AI_SYSTEM_H__
#define __CHERRY_AI_SYSTEM_H__

#include <mzx/ecs.h>

namespace cherry
{

class Scene;

class AISystem : public mzx::EntitySystem<AISystem, void(Scene *)>
{
private:
    virtual void _Update(Scene *scene) override;
};

} // namespace cherry

#endif
