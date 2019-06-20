#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/view.h>

namespace cherry
{

static bool SerializeViewPublish(const mzx::ComponentBase *base,
                                 std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<ViewPublish> *>(base)->Get();
    for (auto &entity : component->subscribe_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

static mzx::ComponentBase *UnserializeViewPublish(const std::string &data)
{
    auto *component = new mzx::Component<ViewPublish>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(ViewPublish, SerializeViewPublish,
                                  UnserializeViewPublish);

static bool SerializeViewSubscribe(const mzx::ComponentBase *base,
                                   std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<ViewSubscribe> *>(base)->Get();
    for (auto &entity : component->publish_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

static mzx::ComponentBase *UnserializeViewSubscribe(const std::string &data)
{
    auto *component = new mzx::Component<ViewSubscribe>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(ViewSubscribe, SerializeViewSubscribe,
                                  UnserializeViewSubscribe);

} // namespace cherry

