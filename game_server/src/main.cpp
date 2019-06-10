#include <world.h>

int main(int argc, char *argv[])
{
    cherry::World::Instance().Init();
    cherry::World::Instance().Run();
    cherry::World::Instance().Uninit();
    return 0;
}
