#ifndef _M_STRONG_TYPEDEF_H_
#define _M_STRONG_TYPEDEF_H_

#include <utility>
#include <type_traits>

#define MSTRONG_TYPEDEF(target_type, new_type) \
    auto m_name_seed_##new_type = [](){}; \
    using new_type = MStrongTypedef<target_type, decltype(m_name_seed_##new_type)>;

template <typename T, typename NameSeed
    , typename Enable = typename std::conditional<!std::is_fundamental<T>::value, std::true_type, std::false_type>::type>
class MStrongTypedef;

//specialization for non fundamental types
template <typename T, typename NameSeed>
class MStrongTypedef<T, NameSeed, std::true_type>
   :public T
{
public:
    template <typename... Args>
    MStrongTypedef(Args&&... args)
        :T(std::forward<Args>(args)...)
    {
    }
};

//specialization for fundamental types
template <typename T, typename NameSeed>
class MStrongTypedef<T, NameSeed, std::false_type>
{
public:
    MStrongTypedef()
    {
    }
    MStrongTypedef(const T t)
        :t_(t)
    {
    }
    MStrongTypedef(const MStrongTypedef &t)
        :t_(t.t_)
    {
    }
    MStrongTypedef& operator=(const MStrongTypedef &rhs)
    {
        t_ = rhs.t_;
        return *this;
    }
    MStrongTypedef& operator=(const T &rhs)
    {
        t_ = rhs;
        return *this;
    }
    operator T&()
    {
        return t;
    }
    operator const T&() const
    {
        return t;
    }
    bool operator==(const strong_typedef & rhs) const { return t == rhs.t; }
    bool operator<(const strong_typedef & rhs) const { return t < rhs.t; }
private:
    T t_;
};

#endif
