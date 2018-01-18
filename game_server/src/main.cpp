#include <iostream>
#include <unistd.h>
#include <mzx/cmd_manager.h>
#include <mzx/singleton.h>

int main(int argc, char *argv[])
{
    int i = 1;
    mzx::CmdManager::Instance().RegistCmd("111111", []() {
        std::cout << "receive 1" << std::endl;
    });
    mzx::CmdManager::Instance().RegistCmd("222222", [&]() {
        std::cout << "receive 2" << std::endl;
        i = 0;
    });
    mzx::CmdManager::Instance().Start();
    while (i != 0)
    {
        mzx::CmdManager::Instance().ExcuteCmd();
    }
    mzx::CmdManager::Instance().StopAndJoin();
    return 0;
}
