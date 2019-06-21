#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/hatred.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeHatredPublish(const mzx::ComponentBase *base,
                                   std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<HatredPublish> *>(base)->Get();
    for (auto &entity : component->subscribe_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

static mzx::ComponentBase *UnserializeHatredPublish(const std::string &data)
{
    auto *component = new mzx::Component<HatredPublish>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(HatredPublish, SerializeHatredPublish,
                                  UnserializeHatredPublish);

static bool SerializeHatredSubscribe(const mzx::ComponentBase *base,
                                     std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<HatredSubscribe> *>(base)->Get();
    for (auto &iter_entity : component->publish_list)
    {
        *data += mzx::Concat("(", iter_entity.second->ID(), ",",
                             iter_entity.first, ");");
    }
    return true;
}

static mzx::ComponentBase *UnserializeHatredSubscribe(const std::string &data)
{
    auto *component = new mzx::Component<HatredSubscribe>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(HatredSubscribe, SerializeHatredSubscribe,
                                  UnserializeHatredSubscribe);

static bool SerializeHatredDistanceCheck(const mzx::ComponentBase *base,
                                         std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<HatredDistanceCheck> *>(base)->Get();
    *data = mzx::ConvertTo<std::string>(component->distance);
    return true;
}

static mzx::ComponentBase *
UnserializeHatredDistanceCheck(const std::string &data)
{
    auto distance = mzx::UnsafeConvertTo<double>(data);
    auto *component = new mzx::Component<HatredDistanceCheck>(distance);
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(HatredDistanceCheck,
                                  SerializeHatredDistanceCheck,
                                  UnserializeHatredDistanceCheck);

} // namespace cherry

