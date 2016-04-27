#ifndef _M_STRONG_TYPEDEF_H_
#define _M_STRONG_TYPEDEF_H_

#define MSTRONG_TYPEDEF(T, U)                                 \
class U                                                       \
{                                                             \
public:                                                       \
    explicit U(const T &t) : t_(t) {};                        \
    explicit U(T &&t) : t_(std::move(t)) {};                  \
    template <typename ...Args>                               \
    explicit U(Args ...args) : t_(args...) {};                \
                                                              \
    U& operator=(const T &t) { t_ = t; return *this; };       \
                                                              \
    operator T&() { return t_; };                             \
    operator const T&() const { return t_; };                 \
private:                                                      \
    T t_;                                                     \
};

#endif
