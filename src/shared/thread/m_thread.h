#ifndef _M_THREAD_H_
#define _M_THREAD_H_

#include <pthread.h>
#include <string>

enum class MThreadError
{
    No = 0,
    Unknown = 1,
    IsRunning = 2,
    CreateFailed = 3,
    JoinFailed = 4,
};

class MThread
{
protected:
    MThread();
    ~MThread();
    MThread(const MThread &) = delete;
    MThread& operator=(const MThread &) = delete;
public:
    MThreadError Start();
    void Stop();
    MThreadError Join();
    MThreadError StopAndJoin();
    MThreadError GetLastError() const;
    const std::string& GetLastErrorMsg() const;
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
    MThreadError last_error_;
    std::string last_error_msg_;
};

#endif
