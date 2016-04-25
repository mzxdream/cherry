#ifndef _M_STRONG_TYPEDEF_H_
#define _M_STRONG_TYPEDEF_H_

#include <type_traits>

#define MSTRONG_TYPEDEF(T, U) \
    typedef MStrongTypedef<T, MStrongTypedef_##OldType2##NewType> NewType;

template <typename T, typename U>
class MStrongTypedef
{
public:
    explicit MStrongTypedef(const T &t)
        :t_(t)
    {
    }
    MStrongTypedef(const MStrongTypedef &rhs)
        :t_(rhs.t_)
    {
    }
    MStrongTypedef& operator=(const T &rhs)
    {

    }
    MStrongTypedef& operator=(const MStrongTypedef &rhs)
    {
        t_ = rhs.t_;
        return *this;
    }
private:
    T t_;
};

#endif
