#include "ecs/system/cmd_handle_system.h"
#include "world.h"

namespace cherry {

bool CmdHandleSystem::Init(World *world)
{
    return true;
}

void CmdHandleSystem::Uninit(World *world)
{

}

void CmdHandleSystem::Update(World *world, int64_t delta_time)
{

}

}

