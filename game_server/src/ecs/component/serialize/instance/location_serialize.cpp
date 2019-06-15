#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/location.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeLocation(const mzx::ComponentBase *base, std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *location = static_cast<const mzx::Component<Location> *>(base)->Get();
    *data =
        mzx::Concat(location->direction.X(), ",", location->direction.Y(), ",",
                    location->direction.Z(), ";", location->position.X(), ",",
                    location->position.Y(), ",", location->position.Z());
    return true;
}

static mzx::ComponentBase *UnserializeLocation(const std::string &data)
{
    auto args = mzx::Split(data, ",;");
    if (args.size() < 6)
    {
        return nullptr;
    }
    mzx::Vector3<int32_t> direction(mzx::UnsafeConvertTo<int32_t>(args[0]),
                                    mzx::UnsafeConvertTo<int32_t>(args[1]),
                                    mzx::UnsafeConvertTo<int32_t>(args[2]));
    mzx::Vector3<int32_t> position(mzx::UnsafeConvertTo<int32_t>(args[3]),
                                   mzx::UnsafeConvertTo<int32_t>(args[4]),
                                   mzx::UnsafeConvertTo<int32_t>(args[5]));
    auto *component = new mzx::Component<Location>(direction, position);
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(Location, SerializeLocation,
                                  UnserializeLocation);

} // namespace cherry
