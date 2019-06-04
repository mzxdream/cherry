#include "ecs/entity_system/cmd_handle_system.h"
#include "ecs/event/event_def.h"
#include "ecs/event/cmd_event.h"
#include "world.h"
#include <iostream>

namespace cherry {

CmdHandleSystem::CmdHandleSystem(World *world)
    : world_(world)
    , cmd_event_id_(mzx::EVENT_ID_INVALID)
{

}

CmdHandleSystem::~CmdHandleSystem()
{

}

bool CmdHandleSystem::_Init()
{
    cmd_event_id_ = world_->GetEventManager().AddListener(EventType::CMD_EVENT, std::bind(&CmdHandleSystem::OnRecvCmd, this, std::placeholders::_1));
    return true;
}

void CmdHandleSystem::_Uninit()
{
    world_->GetEventManager().RemoveListener(EventType::CMD_EVENT, cmd_event_id_);
}

bool CmdHandleSystem::_Configure()
{
    return true;
}

void CmdHandleSystem::_Unconfigure()
{

}

void CmdHandleSystem::_Update(int64_t delta_time)
{
    if (cmd_list_.empty())
    {
        return;
    }
    std::cout << cmd_list_.size() << " :" << delta_time << std::endl;
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

