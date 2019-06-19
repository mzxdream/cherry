#ifndef __CHERRY_ENTITY_EVENT_H__
#define __CHERRY_ENTITY_EVENT_H__

#include <mzx/simple_event.h>
#include <string>

namespace mzx
{

class Entity;

} // namespace mzx

namespace cherry
{

struct EntityAddEvent : public mzx::SimpleEvent<EntityAddEvent>
{
    explicit EntityAddEvent(mzx::Entity *e)
        : entity(e)
    {
    }
    mzx::Entity *entity{nullptr};
};

struct EntityRemoveEvent : public mzx::SimpleEvent<EntityRemoveEvent>
{
    explicit EntityRemoveEvent(mzx::Entity *e)
        : entity(e)
    {
    }
    mzx::Entity *entity{nullptr};
};

} // namespace cherry

#endif
