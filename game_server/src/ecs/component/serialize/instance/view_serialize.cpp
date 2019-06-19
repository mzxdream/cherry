#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/view.h>

namespace cherry
{

static bool SerializeView(const mzx::ComponentBase *base, std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component = static_cast<const mzx::Component<View> *>(base)->Get();
    for (auto &entity : component->see_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

static mzx::ComponentBase *UnserializeView(const std::string &data)
{
    auto *component = new mzx::Component<View>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(View, SerializeView, UnserializeView);

} // namespace cherry

