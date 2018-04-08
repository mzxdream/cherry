#include "ecs/entity/entity.h"

namespace cherry {

Entity::Entity(EntityID id)
    : id_(id)
{
}

Entity::~Entity()
{
    RemoveAllComponent();
}

EntityID Entity::Id() const
{
    return id_;
}

void Entity::RemoveAllComponent()
{
    for (auto iter_component : component_list_)
    {
        delete iter_component.second;
    }
    component_list_.clear();
}

}
