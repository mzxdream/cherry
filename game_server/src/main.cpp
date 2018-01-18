#include <iostream>
#include <unistd.h>
#include <mzx/cmd_manager.h>
#include <mzx/singleton.h>

using CmdManager = mzx::Singleton<mzx::CmdManager>;

int main(int argc, char *argv[])
{
    int i = 1;
    CmdManager::Instance().RegistCmd("111111", []() {
        std::cout << "receive 1" << std::endl;
    });
    CmdManager::Instance().RegistCmd("222222", [&]() {
        std::cout << "receive 2" << std::endl;
        i = 0;
    });
    CmdManager::Instance().Start();
    while (i != 0)
    {
        CmdManager::Instance().ExcuteCmd();
    }
    CmdManager::Instance().StopAndJoin();
    return 0;
}
