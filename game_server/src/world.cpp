#include "world.h"
#include <mzx/system/cmd_line.h>
#include <mzx/system/signal.h>
#include <signal.h>
#include <iostream>

namespace cherry {

World::World()
    : system_manager_(entity_manager_, event_manager_)
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
}

bool World::Init()
{
    mzx::system::Signal::Hook(SIGINT, StopWorld);
    mzx::system::Signal::Hook(SIGTERM, StopWorld);
    mzx::system::CmdLine::Regist("addentity", HandleCmd);
    return true;
}

void World::Uninit()
{
    mzx::system::CmdLine::Stop();
}

void World::Run()
{

    mzx::system::CmdLine::Start();
    while (true)
    {

    }
}

}
