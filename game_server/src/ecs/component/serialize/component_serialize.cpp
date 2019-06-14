#include <ecs/component/serialize/component_serialize.h>

namespace cherry
{

ComponentSerializeFactory::ComponentSerializeFactory()
{
}

ComponentSerializeFactory::~ComponentSerializeFactory()
{
    for (auto &iter_info : serialize_list_)
    {
        delete iter_info.second;
    }
}

bool ComponentSerializeFactory::Regist(
    mzx::ComponentBase::ClassIndexType type, const char *name,
    std::function<bool(const mzx::ComponentBase *, std::string *)>
        serialize_func,
    std::function<mzx::ComponentBase *(const std::string &)> unserialize_func)
{
    if (serialize_list_.find(type) != serialize_list_.end() ||
        serialize_index_.find(name) != serialize_index_.end())
    {
        return false;
    }
    auto *info =
        new ComponentSerializeInfo(name, serialize_func, unserialize_func);
    serialize_list_.emplace(type, info);
    serialize_index_.emplace(name, info);
    return true;
}

const char *ComponentSerializeFactory::Serialize(const mzx::ComponentBase *base,
                                                 std::string *data)
{
    if (!base || !data)
    {
        return nullptr;
    }
    auto iter = serialize_list_.find(base->ClassIndex());
    if (iter == serialize_list_.end() || !iter->second->serialize_func)
    {
        return nullptr;
    }
    if (!(iter->second->serialize_func)(base, data))
    {
        return nullptr;
    }
    return iter->second->name.c_str();
}

mzx::ComponentBase *
ComponentSerializeFactory::Unserialize(const char *name,
                                       const std::string &data)
{
    if (!name)
    {
        return nullptr;
    }
    auto iter = serialize_index_.find(name);
    if (iter == serialize_index_.end() || !iter->second->unserialize_func)
    {
        return nullptr;
    }
    return (iter->second->unserialize_func)(data);
}

} // namespace cherry
