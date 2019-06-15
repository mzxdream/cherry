#ifndef __CHERRY_LOCATION_H__
#define __CHERRY_LOCATION_H__

#include <cstdint>
#include <mzx/data_structure/vector3.h>

namespace cherry
{

struct Location
{
    explicit Location(const mzx::Vector3<int32_t> &d,
                      const mzx::Vector3<int32_t> &p)
        : direction(d)
        , position(p)
    {
    }
    mzx::Vector3<int32_t> direction;
    mzx::Vector3<int32_t> position;
};

} // namespace cherry

#endif
