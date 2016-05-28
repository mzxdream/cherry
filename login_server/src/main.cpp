#include <signal.h>
#include <iostream>

static bool stop_flag = false;

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
    while (!stop_flag)
    {
    }
    UnhookSignals();
    std::cout << "----------end--------" << std::endl;
    return 0;
}
