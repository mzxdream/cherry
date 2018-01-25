#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i = 1;

    mzx::system::Signal::Hook(SIGTERM, [&](int c) {
        printf("sigterm %d", c);
        fflush(stdout);
        i = 0;
    });
    mzx::system::CmdLine::Regist("111111", [](const std::string &str) {
        std::cout << "receive " << str << std::endl;
    });
    mzx::system::CmdLine::Regist("222222", [&](const std::string &str) {
        std::cout << "receive " << str << std::endl;
        i = 0;
    });
    mzx::system::CmdLine::Start();
    while (i != 0)
    {
        mzx::system::CmdLine::Excute();
    }
    mzx::system::CmdLine::Stop();
    return 0;
}
