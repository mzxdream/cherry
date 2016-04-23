#ifndef _M_MATH_H_
#define _M_MATH_H_

#include <climits>
#include <type_traits>

class MMath
{
public:
    template <typename T, typename std::enable_if<std::is_unsigned<T>::value, int>::type = 0>
    static T NextPower2(T val)
    {
        --val;
        for (std::size_t i = 1; i < sizeof(val) * CHAR_BIT; i <<= 1) {
            val |= (val >> i);
        }
        return ++val;
    }
};

#endif
