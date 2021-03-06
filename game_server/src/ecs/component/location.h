#ifndef __CHERRY_LOCATION_H__
#define __CHERRY_LOCATION_H__

#include <cstdint>
#include <mzx/data_structure/vector3.h>

namespace cherry
{

struct Location
{
    explicit Location(const mzx::Vector3<double> &d,
                      const mzx::Vector3<double> &p)
        : direction(d)
        , position(p)
    {
    }
    mzx::Vector3<double> direction;
    mzx::Vector3<double> position;
};

} // namespace cherry

#endif
