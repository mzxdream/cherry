#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/movement.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeMovement(const mzx::ComponentBase *base, std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<Movement> *>(base)->Get();
    *data =
        mzx::Concat(component->destination.X(), ",", component->destination.Y(),
                    ",", component->destination.Z(), ";", component->velocity);
    return true;
}

static mzx::ComponentBase *UnserializeMovement(const std::string &data)
{
    auto args = mzx::Split(data, ",;");
    if (args.size() < 4)
    {
        return nullptr;
    }
    mzx::Vector3<double> destination(mzx::UnsafeConvertTo<double>(args[0]),
                                     mzx::UnsafeConvertTo<double>(args[1]),
                                     mzx::UnsafeConvertTo<double>(args[2]));
    auto velocity = mzx::UnsafeConvertTo<double>(args[3]);
    auto *component = new mzx::Component<Movement>(destination, velocity);
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(Movement, SerializeMovement,
                                  UnserializeMovement);

} // namespace cherry

