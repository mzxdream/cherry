#ifndef __CHERRY_MOVEMENT_H__
#define __CHERRY_MOVEMENT_H__

#include <cstdint>
#include <mzx/vector3.h>

namespace cherry
{

struct Movement
{
    explicit Movement(const mzx::Vector3<double> &dir, double v)
        : direction(dir)
        , velocity(v)
    {
    }
    mzx::Vector3<double> direction;
    double velocity;
};

} // namespace cherry

#endif
