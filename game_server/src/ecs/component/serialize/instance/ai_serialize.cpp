#include <mzx/convert.h>

#include <ecs/component/ai.h>
#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

static bool SerializeAI(const mzx::ComponentBase *base, std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    return true;
}

static mzx::ComponentBase *UnserializeAI(const std::string &data)
{
    auto *component = new mzx::Component<AI>();
    return component;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(AI, SerializeAI, UnserializeAI);

} // namespace cherry

