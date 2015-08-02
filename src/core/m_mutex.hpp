#ifndef _M_MUTEX_H_
#define _M_MUTEX_H_

#include <pthread.h>

class MNoneMutex
{
public:
    MNoneMutex() = default;
    ~MNoneMutex() = default;
    MNoneMutex(const MNoneMutex &) = delete;
    MNoneMutex& operator=(const MNoneMutex &) = delete;
public:
    void Lock(){}
    void Unlock(){}
    bool TryLock()
    {
        return true;
    }
};

class MMutex
{
public:
    MMutex()
    {
        ::pthread_mutex_init(&mtx_, NULL);
    }
    ~MMutex()
    {
        ::pthread_mutex_destroy(&mtx_);
    }
    MMutex(const MMutex &) = delete;
    MMutex& operator=(const MMutex &) = delete;
public:
    void Lock()
    {
        ::pthread_mutex_lock(&mtx_);
    }
    void Unlock()
    {
        ::pthread_mutex_unlock(&mtx_);
    }
    bool TryLock()
    {
        return 0 == ::pthread_mutex_trylock(&mtx_);
    }
private:
    pthread_mutex_t mtx_;
};

template<typename TMutex>
class MAutoLock
{
public:
    explicit MAutoLock(TMutex &r_mtx)
        :r_mtx_(r_mtx)
    {
        r_mtx_.Lock();
    }
    ~MAutoLock()
    {
        r_mtx_.Unlock();
    }
    MAutoLock(const MAutoLock &) = delete;
    MAutoLock& operator=(const MAutoLock &) = delete;
private:
    TMutex &r_mtx_;
};

#endif
