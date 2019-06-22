#include <mzx/random.h>

#include <ecs/component/ai.h>
#include <ecs/component/hatred.h>
#include <ecs/component/location.h>
#include <ecs/component/movement.h>
#include <ecs/system/ai_system.h>
#include <scene/scene.h>
#include <world.h>

namespace cherry
{

void AISystem::_Update(Scene *scene)
{
    scene->GetEntityManager().ForeachEntity<AI, Location>(
        [](mzx::Entity *entity) {
            auto *location = entity->GetComponent<Location>();
            auto *hatred_subscribe = entity->GetComponent<HatredSubscribe>();
            if (hatred_subscribe && !hatred_subscribe->publish_list.empty())
            {
                auto *other = hatred_subscribe->publish_list.begin()->second;
                auto *other_location = other->GetComponent<Location>();
                if (other_location)
                {
                    auto distance = mzx::Vector3<double>::Distance(
                        location->position, other_location->position);
                    if (distance <= 3)
                    {
                        entity->RemoveComponent<Movement>();
                    }
                    else
                    {
                        auto *movement = entity->GetComponent<Movement>();
                        if (movement)
                        {
                            if (movement->destination !=
                                other_location->position)
                            {
                                movement->destination =
                                    other_location->position;
                            }
                        }
                        else
                        {
                            entity->AddComponent<Movement>(
                                other_location->position, 0.05);
                        }
                    }
                }
            }
            else
            {
                auto *movement = entity->GetComponent<Movement>();
                if (!movement)
                {
                    mzx::Vector3<double> destination(mzx::Random(0.0, 400.0),
                                                     mzx::Random(0.0, 400.0),
                                                     mzx::Random(0.0, 400.0));
                    entity->AddComponent<Movement>(destination, 0.001);
                }
            }
            return true;
        });
}

} // namespace cherry
