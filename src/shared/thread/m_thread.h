#ifndef _M_THREAD_H_
#define _M_THREAD_H_

#include <pthread.h>

class MThread
{
protected:
    MThread();
public:
    ~MThread();
    MThread(const MThread &) = delete;
    MThread& operator=(const MThread &) = delete;
public:
    bool Start();
    void Stop();
    bool Join();
    bool StopAndJoin();
private:
    virtual bool DoBeforeThreadStart() { return true; }
    virtual void DoAfterThreadStop() {}
    virtual void DoRun() = 0;
    static void* ThreadMain(void *p_param);
private:
    pthread_t th_;
    volatile bool need_run_;
    volatile bool running_;
};

#endif
