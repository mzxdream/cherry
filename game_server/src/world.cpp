#include <iostream>
#include <mzx/date_time.h>
#include <mzx/system/cmd_line.h>
#include <mzx/system/signal.h>

#include "ecs/entity_system/cmd_handle_system.h"
#include "ecs/event/cmd_event.h"
#include "world.h"

namespace cherry
{

constexpr static int64_t FRAME_TIME = 33 * 100; // TODO

static void HandleSignal(mzx::SignalType type)
{
    std::cout << "receive signal:" << type << std::endl;
    World::Instance().Stop();
}

static void HandleCmd(const std::string &cmd)
{
    std::cout << "cmd:" << cmd << " handle not found" << std::endl;
}

World::World()
{
}

World::~World()
{
}

World::EventManager &World::GetEventManager()
{
    return event_manager_;
}

bool World::Init()
{
    mzx::Signal::Hook(mzx::SignalType::Interrupt, HandleSignal);
    mzx::Signal::Hook(mzx::SignalType::Terminal, HandleSignal);
    mzx::CmdLine::Regist(HandleCmd);
    cur_time_ = mzx::DateTime::NowMilliseconds();
    stop_flag_ = false;
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
    while (!stop_flag_)
    {
        auto next_time = mzx::DateTime::NowMilliseconds();
        while (!stop_flag_ && cur_time_ + FRAME_TIME <= next_time)
        {
            cur_time_ += FRAME_TIME;
            scene_manager_.Update();
            mzx::CmdLine::Execute();
        }
        if (!stop_flag_)
        {
            mzx::Thread::Sleep(cur_time_ + FRAME_TIME - next_time);
        }
    }
    mzx::CmdLine::Stop();
}

int64_t World::CurTime() const
{
    return cur_time_;
}

} // namespace cherry
