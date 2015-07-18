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
            :p_instance_(NULL)
        {
        }
        ~MSingletonBase()
        {
            if (p_instance_)
            {
                delete p_instance_;
                p_instance_ = NULL;
            }
        }
    private:
        MSingletonBase(const MSingletonBase &);
        MSingletonBase& operator=(const MSingleton &);
    public:
        TClass& Instance()
        {
            if (!p_instance_)
            {
                MAutoLock<TMutex> lock(mtx_);
                if (!p_instance_)
                {
                    p_instance_ = new TClass();
                }
            }
            return *p_instance_;
        }
    private:
        TClass *p_instance_;
        TMutex mtx_;
    };
private:
    static MSingletonBase instance_;
};

template<typename TClass, typename TMutex>
typename MSingleton<TClass, TMutex>::MSingletonBase MSingleton<TClass, TMutex>::instance_;

#endif
