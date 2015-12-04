#include <thread/m_thread.h>
#include <util/m_string.h>

MThread::MThread()
    :need_run_(false)
    ,running_(false)
    ,last_error_(MThreadError::No)
{
}

MThread::~MThread()
{
    StopAndJoin();
}

MThreadError MThread::Start()
{
    if (need_run_
        || running_)
    {
        last_error_msg_ = "thread is running";
        last_error_ = MThreadError::IsRunning;
        return last_error_;
    }
    need_run_ = true;
    running_ = true;
    int err = pthread_create(&th_, nullptr, &ThreadMain, this);
    if (err != 0)
    {
        need_run_ = false;
        running_ = false;
        last_error_msg_ = MConcat("create thread failed error:", err);
        last_error_ = MThreadError::CreateFailed;
        return last_error_;
    }
    last_error_msg_ = "";
    last_error_ = MThreadError::No;
    return last_error_;
}

void MThread::Stop()
{
    if (need_run_)
    {
        need_run_ = false;
    }
}

MThreadError MThread::Join()
{
    if (running_)
    {
        int err = pthread_join(th_, nullptr);
        if (err != 0)
        {
            last_error_msg_ = MConcat("join thread failed err:", err);
            last_error_ = MThreadError::JoinFailed;
            return last_error_;
        }
    }
    last_error_msg_ = "";
    last_error_ = MThreadError::No;
    return last_error_;
}

MThreadError MThread::StopAndJoin()
{
    Stop();
    return Join();
}

MThreadError MThread::GetLastError() const
{
    return last_error_;
}

const std::string& MThread::GetLastErrorMsg() const
{
    return last_error_msg_;
}

void* MThread::ThreadMain(void *p_param)
{
    if (!p_param)
    {
        return nullptr;
    }
    MThread *p_th = static_cast<MThread*>(p_param);
    if (!p_th)
    {
        return nullptr;
    }
    do
    {
        if (!p_th->DoBeforeThreadStart())
        {
            p_th->last_error_msg_ = "do before thread start failed";
            p_th->last_error_ = MThreadError::Unknown;
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
