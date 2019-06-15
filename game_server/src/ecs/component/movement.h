#ifndef __CHERRY_MOVEMENT_H__
#define __CHERRY_MOVEMENT_H__

#include <cstdint>
#include <mzx/data_structure/vector3.h>

namespace cherry
{

struct Movement
{
    explicit Movement(const mzx::Vector3<int32_t> &dest, int32_t v)
        : destination(dest)
        , velocity(v)
    {
    }
    mzx::Vector3<int32_t> destination;
    int32_t velocity;
};

} // namespace cherry

#endif
