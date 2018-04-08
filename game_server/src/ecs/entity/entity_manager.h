#ifndef __CHERRY_ENTITY_MANAGER_H__
#define __CHERRY_ENTITY_MANAGER_H__

#include <functional>

#include "ecs/entity/entity.h"

namespace cherry {

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager &) = delete;
    EntityManager & operator=(const EntityManager &) = delete;
public:
    Entity * CreateEntity();
    void DestroyEntity(EntityID id);
    void DestroyAllEntity();
    Entity * GetEntity(EntityID id);
    template <typename T, typename ...Args>
    void ForeachEntity(const std::function<void (Entity *)> &cb)
    {
        for (auto &iter_entity : entity_list_)
        {
            if (!iter_entity.second->HasComponent<T, Args...>())
            {
                continue;
            }
            cb(iter_entity.second);
        }
    }
    void ForeachEntity(const std::function<void (Entity *)> &cb);
private:
    EntityID next_entity_id_;
    std::map<EntityID, Entity *> entity_list_;
};

}

#endif
