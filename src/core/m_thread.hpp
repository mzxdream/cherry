#ifndef _M_THREAD_H_
#define _M_THREAD_H_

#include <pthread.h>

class MThread
{
protected:
    MThread()
        :thread_run_(false)
        ,thread_status_(ThreadStatusInit)
    {
    }
public:
    virtual ~MThread()
    {
        Stop();
    }
    MThread(const MThread &) = delete;
    MThread& operator=(const MThread &) = delete;
public:
    enum ThreadStatus{ThreadStatusInit = 0, ThreadStatusRunnable, ThreadStatusRunning, ThreadStatusSuspend, ThreadStatusDead};
public:
    bool Start()
    {
        if (thread_status_ != ThreadStatusInit
            || thread_status_ != ThreadStatusDead)
        {
            return false;
        }
        thread_status_ = ThreadStatusRunnable;
        thread_run_ = true;
        if (::pthread_create(&thread_, NULL, &ThreadMain, this) != 0)
        {
            thread_status_ = ThreadStatusDead;
            thread_run_ = false;
            return false;
        }
        return true;
    }
    void Stop()
    {
        thread_run_ = false;
        if (thread_status_ != ThreadStatusInit
            || thread_status_ != ThreadStatusDead)
        {
            ::pthread_join(thread_, NULL);
        }
    }
protected:
    virtual void OnBeforeThreadRun(){}
    virtual void OnAfterThreadRun(){}
    virtual void OnBeforeThreadLoop(){}
    virtual void OnAfterThreadLoop(){}
    virtual void ThreadRun() = 0;
private:
    static void* ThreadMain(void *p_param)
    {
        MThread *p_th = static_cast<MThread*>(p_param);
        if (p_th)
        {
            p_th->OnBeforeThreadRun();
            p_th->thread_status_ = ThreadStatusRunning;
            while (p_th->thread_run_)
            {
                p_th->OnBeforeThreadLoop();
                p_th->ThreadRun();
                p_th->OnAfterThreadLoop();
            }
            p_th->thread_status_ = ThreadStatusDead;
            p_th->OnAfterThreadRun();
        }
        ::pthread_exit(NULL);
        return NULL;
    }
private:
    pthread_t thread_;
    volatile bool thread_run_;
    volatile ThreadStatus thread_status_;
};

#endif
