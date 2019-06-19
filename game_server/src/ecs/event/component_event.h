#ifndef __CHERRY_COMPONENT_EVENT_H__
#define __CHERRY_COMPONENT_EVENT_H__

#include <mzx/simple_event.h>
#include <string>

namespace mzx
{

class Entity;
class ComponentBase;

} // namespace mzx

namespace cherry
{

struct ComponentAddEvent : public mzx::SimpleEvent<ComponentAddEvent>
{
    explicit ComponentAddEvent(mzx::Entity *e, mzx::ComponentBase *base)
        : entity(e)
        , component_base(base)
    {
    }
    mzx::Entity *entity{nullptr};
    mzx::ComponentBase *component_base{nullptr};
};

struct ComponentRemoveEvent : public mzx::SimpleEvent<ComponentRemoveEvent>
{
    explicit ComponentRemoveEvent(mzx::Entity *e, mzx::ComponentBase *base)
        : entity(e)
        , component_base(base)
    {
    }
    mzx::Entity *entity{nullptr};
    mzx::ComponentBase *component_base{nullptr};
};

} // namespace cherry

#endif
