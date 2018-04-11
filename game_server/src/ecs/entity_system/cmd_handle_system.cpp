#include "ecs/entity_system/cmd_handle_system.h"
#include "ecs/event/cmd_event.h"
#include "world.h"
#include <iostream>

namespace cherry {

CmdHandleSystem::CmdHandleSystem(World *world)
    : world_(world)
{

}

CmdHandleSystem::~CmdHandleSystem()
{

}

bool CmdHandleSystem::Init()
{
    return true;
}

void CmdHandleSystem::Uninit()
{

}

void CmdHandleSystem::Update(int64_t delta_time)
{
    std::cout << cmd_list_.size() << std::endl;
    for (auto &cmd : cmd_list_)
    {
        std::cout << cmd << std::endl;
    }
    cmd_list_.clear();
}

void CmdHandleSystem::OnRecvCmd(const Event *event)
{
    auto cmd_event = static_cast<const CmdEvent *>(event);
    cmd_list_.push_back(cmd_event->GetCmd());
}

}

