#include "m_mem_tracer.hpp"
#include "m_singleton.hpp"

class T
    :public MSingleton<T, MMutex>
{
};

int main()
{
    T::Instance();
    //int *a = new int();
//    int *b = new int[100];
    //char *c = (char*)malloc(100);
//    delete a;
//    delete[] b;
    return 0;
}
