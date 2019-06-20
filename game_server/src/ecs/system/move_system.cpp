#include <ecs/component/location.h>
#include <ecs/component/movement.h>
#include <ecs/event/position_change_event.h>
#include <ecs/system/move_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

static bool UpdateLocation(Scene *scene, mzx::Entity *entity)
{
    auto delta_time = World::Instance().DeltaTime();
    auto *location = entity->GetComponent<Location>();
    auto *movement = entity->GetComponent<Movement>();
    if (location->position == movement->destination)
    {
        entity->RemoveComponent<Movement>();
        return true;
    }
    location->direction = mzx::Vector3<double>::Normalize(
        movement->destination - location->position);
    auto distance = mzx::Vector3<double>::Distance(location->position,
                                                   movement->destination);
    auto last_position = location->position;
    if (distance <= delta_time * movement->velocity)
    {
        location->position = movement->destination;
        entity->RemoveComponent<Movement>();
    }
    else
    {
        location->position +=
            location->direction * delta_time * movement->velocity;
    }
    scene->GetEventManager().InvokeEmplace<PositionChangeEvent>(entity,
                                                                last_position);
    return true;
}

void MoveSystem::_Update(Scene *scene)
{
    scene->GetEntityManager().ForeachEntity<Location, Movement>(
        [&scene](mzx::Entity *entity) {
            return UpdateLocation(scene, entity);
        });
}

} // namespace cherry
