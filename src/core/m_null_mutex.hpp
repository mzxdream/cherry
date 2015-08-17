#ifndef _M_NULL_MUTEX_HPP_
#define _M_NULL_MUTEX_HPP_

class MNullMutex
{
public:
    void lock(){}
    void unlock(){}
    bool try_lock()
    {
        return true;
    }
};

#endif
