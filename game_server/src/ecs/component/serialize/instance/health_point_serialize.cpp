#include <mzx/convert.h>

#include <ecs/component/health_point.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeHealthPoint(const mzx::ComponentBase *base,
                                 std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    *data = mzx::UnsafeConvertTo<std::string>(
        static_cast<const mzx::Component<HealthPoint> *>(base)->Get()->hp);
    return true;
}

static mzx::ComponentBase *UnserializeHealthPoint(const std::string &data)
{
    auto *component =
        new mzx::Component<HealthPoint>(mzx::UnsafeConvertTo<int64_t>(data));
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(HealthPoint, SerializeHealthPoint,
                                  UnserializeHealthPoint);

} // namespace cherry
