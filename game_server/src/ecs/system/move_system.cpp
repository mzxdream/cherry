#include <ecs/component/location.h>
#include <ecs/component/movement.h>
#include <ecs/system/move_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

void MoveSystem::_Update(Scene *scene)
{
    auto delta_time = World::Instance().DeltaTime();
    scene->GetEntityManager().ForeachEntity<Location, Movement>(
        [&delta_time](mzx::Entity *entity) {
            auto *location = entity->GetComponent<Location>();
            auto *movement = entity->GetComponent<Movement>();
            auto len =
                (movement->destination - location->position).SqrtMagnitude();
            if (len <= delta_time * movement->velocity)
            {
                location->position = movement->destination;
                entity->RemoveComponent<Movement>();
            }
            else
            {
                auto direction = movement->destination - location->position;
                direction.Normalize();
                location->position +=
                    direction * delta_time * movement->velocity / 1000;
            }
            return true;
        });
}

} // namespace cherry
