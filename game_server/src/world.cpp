#include "world.h"
#include <mzx/system/cmd_line.h>
#include <mzx/system/signal.h>
#include <mzx/time_util.h>
#include <iostream>
#include "ecs/event/cmd_event.h"
#include "ecs/entity_system/cmd_handle_system.h"

namespace cherry {

void HandleSignal(mzx::SignalType type)
{
    std::cout << "receive signal:" << type << std::endl;
    World::Instance().Stop();
}

void HandleCmd(const std::string &cmd)
{
    if (cmd == "addscene")
    {

    }
    else if (cmd == "addentity")
    {

    }
}

World::World()
    : stop_flag_(false)
    , cur_time_(0)
{

}

World::~World()
{

}

World::EventManager & World::GetEventManager()
{
    return event_manager_;
}

bool World::Init()
{
    mzx::Signal::Hook(SIGINT, HandleSignal);
    mzx::Signal::Hook(SIGTERM, HandleSignal);
    mzx::CmdLine::Regist(HandleCmd);
    return true;
}

void World::Uninit()
{
    mzx::CmdLine::UnregistAll();
    mzx::Signal::UnhookAll();
}

void World::Stop()
{
    stop_flag_ = true;
}

void World::Run()
{
    mzx::CmdLine::Start();
    stop_flag_ = false;
    cur_time_ = mzx::TimeUtil::Now();
    int64_t frame_time = 16 * 1000;
    int64_t delta_time = 0;
    while (!stop_flag_)
    {
        mzx::CmdLine::Execute();
        scene_manager_.Update(delta_time);
        while (!stop_flag_)
        {
            int64_t now_time = mzx::TimeUtil::Now();
            delta_time = now_time - cur_time_;
            if (delta_time >= frame_time)
            {
                cur_time_ = now_time;
                break;
            }
            mzx::TimeUtil::Sleep(frame_time - delta_time);
        }
    }
    mzx::CmdLine::Stop();
}

int64_t World::CurTime() const
{
    return cur_time_;
}

}
