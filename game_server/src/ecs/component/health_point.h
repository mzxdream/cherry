#ifndef __CHERRY_HEALTH_POINT_H__
#define __CHERRY_HEALTH_POINT_H__

#include <cstdint>

namespace cherry
{

struct HealthPoint
{
    explicit HealthPoint(int64_t h = 0)
        : hp(h)
    {
    }

    int64_t hp;
};

} // namespace cherry

#endif
