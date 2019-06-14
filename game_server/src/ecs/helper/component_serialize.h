#ifndef __MZX_COMPONENT_SERIALIZE_H__
#define __MZX_COMPONENT_SERIALIZE_H__

#include <mzx/ecs.h>
#include <mzx/singleton.h>

namespace cherry
{

struct ComponentSerializeInfo final
{
    explicit ComponentSerializeInfo(
        const std::string &n,
        std::function<bool(const mzx::ComponentBase *, std::string *)>
            serialize,
        std::function<mzx::ComponentBase *(const std::string &)> unserialize)
        : name(n)
        , serialize_func(serialize)
        , unserialize_func(unserialize)
    {
    }
    std::string name;
    std::function<bool(const mzx::ComponentBase *, std::string *)>
        serialize_func;
    std::function<mzx::ComponentBase *(const std::string &)> unserialize_func;
};

class ComponentSerializeFactory
    : public mzx::Singleton<ComponentSerializeFactory>
{
    friend mzx::Singleton<ComponentSerializeFactory>;

private:
    ComponentSerializeFactory();
    ~ComponentSerializeFactory();

public:
    bool Regist(mzx::ComponentBase::ClassIndexType type, const char *name,
                std::function<bool(const mzx::ComponentBase *, std::string *)>
                    serialize_func,
                std::function<mzx::ComponentBase *(const std::string &)>
                    unserialize_func);
    const char *Serialize(const mzx::ComponentBase *base, std::string *data);
    mzx::ComponentBase *Unserialize(const char *name, const std::string &data);

private:
    std::unordered_map<mzx::ComponentBase::ClassIndexType,
                       ComponentSerializeInfo *>
        serialize_list_;
    std::unordered_map<std::string, const ComponentSerializeInfo *>
        serialize_index_;
};

#define CHERRY_COMPONENT_SERIALIZE_REGIST(T, serialize, unserialize)           \
    class ComponentSerializeRegister##T                                        \
    {                                                                          \
        const static bool is_regist_;                                          \
    };                                                                         \
    const bool ComponentSerializeRegister##T::is_regist_ =                     \
        ComponentSerializeFactory::Instance().Regist(                          \
            mzx::Component<T>::CLASS_INDEX, #T, serialize, unserialize);

}; // namespace cherry

#endif
