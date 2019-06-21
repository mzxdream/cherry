#ifndef __CHERRY_HATRED_SYSTEM_H__
#define __CHERRY_HATRED_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/simple_event.h>

namespace cherry
{

class Scene;

class HatredSystem : public mzx::EntitySystem<HatredSystem, void(Scene *)>
{
public:
    explicit HatredSystem(Scene *scene);
    virtual ~HatredSystem();

    void OnRemoveComponent(const mzx::SimpleEventBase *base);

private:
    virtual void _Update(Scene *scene) override;

private:
    Scene *scene_{nullptr};
};

} // namespace cherry

#endif
