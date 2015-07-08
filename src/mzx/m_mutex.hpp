#ifndef _M_MUTEX_H_
#define _M_MUTEX_H_

#include <pthread.h>

class MNoneMutex
{
public:
    MNoneMutex(){}
    ~MNoneMutex(){}
private:
    MNoneMutex(const MNoneMutex &);
    MNoneMutex& operator=(const MNoneMutex &);
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
private:
    MMutex(const MMutex &);
    MMutex& operator=(const MMutex &);
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
    explicit MAutoLock(TMutex &mtx)
        :mtx_(mtx)
    {
        mtx_.Lock();
    }
    ~MAutoLock()
    {
        mtx_.Unlock();
    }
private:
    MAutoLock(const MAutoLock &);
    MAutoLock& operator=(const MAutoLock &);
private:
    TMutex &mtx_;
};

#endif
