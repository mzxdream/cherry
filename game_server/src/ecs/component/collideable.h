#ifndef __CHERRY_COLLIDEABLE_H__
#define __CHERRY_COLLIDEABLE_H__

#include <cstdint>

namespace cherry {

struct Collideable
{
    explicit Collideable(double r = 0)
        : radius(r)
    {

    }
    double radius;
};

}

#endif
