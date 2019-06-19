#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/appear.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeAppear(const mzx::ComponentBase *base, std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component = static_cast<const mzx::Component<Appear> *>(base)->Get();
    for (auto &entity : component->follower_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

static mzx::ComponentBase *UnserializeAppear(const std::string &data)
{
    auto *component = new mzx::Component<Appear>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(Appear, SerializeAppear, UnserializeAppear);

} // namespace cherry
