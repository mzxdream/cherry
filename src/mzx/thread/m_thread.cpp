#include <mzx/thread/m_thread.h>
#include <mzx/util/m_log.h>

MThread::MThread()
    :need_run_(false)
    ,running_(false)
{
}

MThread::~MThread()
{
    StopAndJoin();
}

bool MThread::Start()
{
    if (need_run_
        || running_)
    {
        MLOG(Error) << "thread is running";
        return false;
    }
    need_run_ = true;
    running_ = true;
    int err = pthread_create(&th_, nullptr, &ThreadMain, this);
    if (err != 0)
    {
        need_run_ = false;
        running_ = false;
        MLOG(Error) << "create thread failed:" << err;
        return false;
    }
    return true;
}

void MThread::Stop()
{
    if (need_run_)
    {
        need_run_ = false;
    }
}

bool MThread::Join()
{
    if (running_)
    {
        int err = pthread_join(th_, nullptr);
        if (err != 0)
        {
            MLOG(Error) << "join thread failed err:" << err;
            return false;
        }
    }
    return true;
}

bool MThread::StopAndJoin()
{
    Stop();
    return Join();
}

void* MThread::ThreadMain(void *p_param)
{
    do
    {
        if (!p_param)
        {
            MLOG(Error) << "p_param is null";
            break;
        }
        MThread *p_th = static_cast<MThread*>(p_param);
        if (!p_th)
        {
            MLOG(Error) << "p_th cast from p_param is null";
            break;
        }
        if (!p_th->DoBeforeThreadStart())
        {
            MLOG(Error) << "do before thread start failed";
            p_th->running_ = false;
            break;
        }
        while (p_th->need_run_)
        {
            p_th->DoRun();
        }
        p_th->DoAfterThreadStop();
        p_th->running_ = false;
    } while (0);
    pthread_exit(nullptr);
    return nullptr;
}
