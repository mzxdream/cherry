#include "world.h"
#include <mzx/system/cmd_line.h>
#include <mzx/system/signal.h>
#include <mzx/time_util.h>
#include <signal.h>
#include <iostream>

namespace cherry {

World::World()
    : stop_flag_(false)
    , cur_time_(0)
    , system_manager_(entity_manager_, event_manager_)
{
}

World::~World()
{
    Uninit();
}

void HandleSignal(mzx::system::Signal::Type type)
{
    std::cout << "receive signal:" << type << std::endl;
    World::Instance().Stop();
}

void HandleCmd(const std::string &cmd)
{
    std::cout << cmd << std::endl;
}

bool World::Init()
{
    mzx::system::Signal::Hook(SIGINT, HandleSignal);
    mzx::system::Signal::Hook(SIGTERM, HandleSignal);
    mzx::system::CmdLine::Regist("addentity", HandleCmd);
    return true;
}

void World::Uninit()
{
    mzx::system::CmdLine::UnregistAll();
    mzx::system::Signal::UnhookAll();
}

void World::Stop()
{
    stop_flag_ = true;
}

void World::Run()
{
    mzx::system::CmdLine::Start();
    stop_flag_ = false;
    cur_time_ = mzx::TimeUtil::Now();
    int64_t frame_time = 16;
    while (!stop_flag_)
    {
        mzx::system::CmdLine::Execute();
        int64_t cost_time = mzx::TimeUtil::Now() - cur_time_;
        if (cost_time > 0 && cost_time < frame_time)
        {
            mzx::TimeUtil::Sleep(frame_time - cost_time);
        }
        cur_time_ = mzx::TimeUtil::Now();
    }
    mzx::system::CmdLine::Stop();
}

int64_t World::CurTime() const
{
    return cur_time_;
}

}
