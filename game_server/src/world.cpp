#include "world.h"
#include <mzx/system/cmd_line.h>
#include <mzx/system/signal.h>
#include <mzx/time_util.h>
#include <iostream>

namespace cherry {

World::World()
    : stop_flag_(false)
    , cur_time_(0)
{
}

World::~World()
{
    Uninit();
}

void HandleSignal(mzx::Signal::Type type)
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
    mzx::Signal::Hook(SIGINT, HandleSignal);
    mzx::Signal::Hook(SIGTERM, HandleSignal);
    mzx::CmdLine::Regist("addentity", HandleCmd);
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
    int64_t frame_time = 16;
    while (!stop_flag_)
    {
        mzx::CmdLine::Execute();
        int64_t cost_time = mzx::TimeUtil::Now() - cur_time_;
        if (cost_time > 0 && cost_time < frame_time)
        {
            mzx::TimeUtil::Sleep(frame_time - cost_time);
        }
        cur_time_ = mzx::TimeUtil::Now();
    }
    mzx::CmdLine::Stop();
}

int64_t World::CurTime() const
{
    return cur_time_;
}

}
