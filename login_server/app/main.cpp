#include <signal.h>
#include <iostream>
#include "login_server.h"
#include <string>

static bool stop_flag = false;
static const std::string config = "./config.json";

void OnSignal(int s)
{
    std::cout << "OnSignal:" << s << std::endl;
    switch (s)
    {
    case SIGINT:
    case SIGTERM:
        stop_flag = true;
        break;
    default:
        break;
    }
    signal(s, OnSignal);
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
    std::cout << "---------begin-------" << std::endl;
    HookSignals();
    if (!LoginServer::Instance().Init(config))
    {
        std::cerr << "init failed" << std::endl;
        return 0;
    }
    while (!stop_flag)
    {
        if (!LoginServer::Instance().Update())
        {
            std::cerr << "update failed" << std::endl;
            break;
        }
    }
    LoginServer::Instance().Clear();
    UnhookSignals();
    std::cout << "----------end--------" << std::endl;
    return 0;
}
