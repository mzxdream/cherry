#include <iostream>
#include <unistd.h>
#include <mzx/cmd_manager.h>

int main(int argc, char *argv[])
{
    mzx::CmdManager cmd_manager;
    cmd_manager.Start();
    int i = 1;
    cmd_manager.RegistCmd("111111", []() {
        std::cout << "receive 1" << std::endl;
    });
    cmd_manager.RegistCmd("222222", [&]() {
        std::cout << "receive 2" << std::endl;
        i = 0;
    });
    while (i != 0)
    {
        cmd_manager.ExcuteCmd();
        sleep(1);
    }
    cmd_manager.StopAndJoin();
    return 0;
}
