#include <iostream>
#include <mzx/date_time.h>
#include <mzx/system/signal.h>
#include <mzx/thread.h>

#include <cmd/cmd_handler.h>
#include <world.h>

namespace cherry
{

constexpr static int64_t FRAME_TIME = 33; // TODO

static void HandleSignal(mzx::SignalType type)
{
    std::cout << "receive signal:" << type << std::endl;
    World::Instance().Stop();
}

World::World()
{
}

World::~World()
{
}

SceneManager &World::GetSceneManager()
{
    return scene_manager_;
}

World::EventManager &World::GetEventManager()
{
    return event_manager_;
}

int64_t World::CurTime() const
{
    return cur_time_;
}

bool World::Init()
{
    mzx::Signal::Hook(mzx::SignalType::Interrupt, HandleSignal);
    mzx::Signal::Hook(mzx::SignalType::Terminal, HandleSignal);
    cur_time_ = mzx::DateTime::NowMilliseconds();
    stop_flag_ = false;
    return true;
}

void World::Uninit()
{
    mzx::Signal::UnhookAll();
}

void World::Stop()
{
    stop_flag_ = true;
}

void World::Run()
{
    CmdHandler::Instance().Start();
    while (!stop_flag_)
    {
        auto next_time = mzx::DateTime::NowMilliseconds();
        while (!stop_flag_ && cur_time_ + FRAME_TIME <= next_time)
        {
            cur_time_ += FRAME_TIME;
            scene_manager_.Update();
            CmdHandler::Instance().Execute();
        }
        if (!stop_flag_)
        {
            mzx::Thread::Sleep(cur_time_ + FRAME_TIME - next_time);
        }
    }
    CmdHandler::Instance().Stop();
}
} // namespace cherry
