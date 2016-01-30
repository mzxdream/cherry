#ifndef _M_THREAD_H_
#define _M_THREAD_H_

#include <pthread.h>
#include <string>
#include <util/m_errno.h>

class MThread
{
protected:
    MThread();
    ~MThread();
    MThread(const MThread &) = delete;
    MThread& operator=(const MThread &) = delete;
public:
    MError Start();
    void Stop();
    MError Join();
    MError StopAndJoin();
private:
    virtual bool DoBeforeThreadStart() { return true; }
    virtual void DoAfterThreadStop() {}
    virtual void DoRun() = 0;
private:
    static void* ThreadMain(void *p_param);
private:
    pthread_t th_;
    bool need_run_;
    bool running_;
};

#endif
