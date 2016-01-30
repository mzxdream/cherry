#include <thread/m_thread.h>
#include <util/m_string.h>
#include <util/m_logger.h>

MThread::MThread()
    :need_run_(false)
    ,running_(false)
{
}

MThread::~MThread()
{
    StopAndJoin();
}

MError MThread::Start()
{
    if (need_run_
        || running_)
    {
        MLOG(MGetLibLogger(), MERR, "thread is running");
        return MError::Running;
    }
    need_run_ = true;
    running_ = true;
    int err = pthread_create(&th_, nullptr, &ThreadMain, this);
    if (err != 0)
    {
        need_run_ = false;
        running_ = false;
        MLOG(MGetLibLogger(), MERR, "create thread failed error:", err);
        return MError::Unknown;
    }
    return MError::No;
}

void MThread::Stop()
{
    if (need_run_)
    {
        need_run_ = false;
    }
}

MError MThread::Join()
{
    if (running_)
    {
        int err = pthread_join(th_, nullptr);
        if (err != 0)
        {
            MLOG(MGetLibLogger(), MERR, "join thread failed err:", err);
            return MError::Unknown;
        }
    }
    return MError::No;
}

MError MThread::StopAndJoin()
{
    Stop();
    return Join();
}

void* MThread::ThreadMain(void *p_param)
{
    MThread *p_th = static_cast<MThread*>(p_param);
    if (!p_th)
    {
        MLOG(MGetLibLogger(), MERR, "param is null");
        return nullptr;
    }
    do
    {
        if (!p_th->DoBeforeThreadStart())
        {
            MLOG(MGetLibLogger(), MERR, "do before thread start failed");
            break;
        }
        while (p_th->need_run_)
        {
            p_th->DoRun();
        }
        p_th->DoAfterThreadStop();
    } while (0);
    p_th->running_ = false;
    pthread_exit(nullptr);
    return nullptr;
}
