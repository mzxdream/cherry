#ifndef __CHERRY_POSITION_CHANGE_EVENT_H_
#define __CHERRY_POSITION_CHANGE_EVENT_H_

#include <mzx/data_structure/vector3.h>
#include <mzx/simple_event.h>

namespace mzx
{

class Entity;

}

namespace cherry
{

struct PositionChangeEvent : public mzx::SimpleEvent<PositionChangeEvent>
{
    explicit PositionChangeEvent(mzx::Entity *e, const mzx::Vector3<double> &p)
        : entity(e)
        , last_position(p)
    {
    }
    mzx::Entity *entity{nullptr};
    mzx::Vector3<double> last_position;
};

} // namespace cherry

#endif
