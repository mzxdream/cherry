#include <net/net_manager.h>
#include <iostream>

int main(int argc, char *argv[])
{
    NetManager net;
    if (!net.Init(4))
    {
        return 0;
    }
    int t = 0;
    cin >> t;
    net.Close();
    return 0;
}
