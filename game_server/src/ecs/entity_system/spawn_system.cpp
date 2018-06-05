#include "ecs/entity_system/spawn_system.h"

namespace cherry {

SpawnSystem::SpawnSystem(Scene *scene)
    : scene_(scene)
{

}

SpawnSystem::~SpawnSystem()
{

}

bool SpawnSystem::_Init()
{
    return true;
}

void SpawnSystem::_Uninit()
{

}

void SpawnSystem::_Update(int64_t now_time)
{
    if (!scene_)
    {
        return;
    }
    auto &entity_manager = scene_->GetEntityManager();

}

bool SpawnSystem::_Configure()
{
    return true;
}

void SpawnSystem::_Unconfigure()
{

}

}
