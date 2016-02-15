#include <signal.h>
#include <iostream>
#include <login_server.h>

static bool sg_run = true;

void OnSignal(int s)
{
    switch (s)
    {
    case SIGINT:
    case SIGTERM:
        sg_run = false;
        break;
    default:
        break;
    }
    std::cout << "recv signal:" << s << std::endl;
}

void HookSignals()
{
    signal(SIGINT, OnSignal);
    signal(SIGTERM, OnSignal);
}

void UnhookSignals()
{
    signal(SIGINT, 0);
    signal(SIGTERM, 0);
}

int main(int argc, char **argv)
{
    HookSignals();
    if (!LoginServer::Instance().Init())
    {
        return 0;
    }
    while (sg_run && LoginServer::Instance().Update())
    {
    }
    LoginServer::Instance().Clear();
    UnhookSignals();
    std::cout << "end....." << std::endl;
    return 0;
}
