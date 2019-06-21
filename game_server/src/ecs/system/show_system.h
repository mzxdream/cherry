#ifndef __CHERRY_SHOW_SYSTEM_H__
#define __CHERRY_SHOW_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/simple_event.h>

namespace cherry
{

class Scene;

class ShowSystem : public mzx::EntitySystem<ShowSystem, void(Scene *)>
{
public:
    explicit ShowSystem(Scene *scene);
    virtual ~ShowSystem();

    void OnAddEntity(const mzx::SimpleEventBase *base);
    void OnRemoveEntity(const mzx::SimpleEventBase *base);
    void OnAddComponent(const mzx::SimpleEventBase *base);
    void OnRemoveComponent(const mzx::SimpleEventBase *base);

private:
    virtual void _Update(Scene *scene) override;

private:
    Scene *scene_{nullptr};
};

} // namespace cherry

#endif
