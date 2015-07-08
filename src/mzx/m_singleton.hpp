#ifndef _M_SINGLETON_H_
#define _M_SINGLETON_H_

#include <iostream>
#include "m_mutex.hpp"

template<typename TClass, typename TMutex>
class MSingleton
{
protected:
    MSingleton(){}
    ~MSingleton(){}
private:
    MSingleton(const MSingleton &);
    MSingleton& operator=(const MSingleton &);
public:
    static TClass& Instance()
    {
        return instance_.Instance();
    }
private:
    class MSingletonBase
    {
    public:
        MSingletonBase()
            :instance_(NULL)
        {
        }
        ~MSingletonBase()
        {
            if (instance_)
            {
                delete instance_;
                instance_ = NULL;
            }
        }
    private:
        MSingletonBase(const MSingletonBase &);
        MSingletonBase& operator=(const MSingleton &);
    public:
        TClass& Instance()
        {
            if (!instance_)
            {
                MAutoLock<TMutex> lock(mtx_);
                if (!instance_)
                {
                    instance_ = new TClass();
                }
            }
            return *instance_;
        }
    private:
        TClass *instance_;
        TMutex mtx_;
    };
private:
    static MSingletonBase instance_;
};

template<typename TClass, typename TMutex>
typename MSingleton<TClass, TMutex>::MSingletonBase MSingleton<TClass, TMutex>::instance_;

#endif
