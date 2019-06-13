#ifndef __CHERRY_HEALTH_POINT_H__
#define __CHERRY_HEALTH_POINT_H__

#include <cstdint>

#include <ecs/helper/component_serialize.h>

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

static bool SerializeHealthPoint(const mzx::ComponentBase *base,
                                 std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    return true;
}

static mzx::ComponentBase *UnserializeHealthPoint(const std::string &data)
{
    return nullptr;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(HealthPoint, SerializeHealthPoint,
                                  UnserializeHealthPoint);

} // namespace cherry

#endif
