#ifndef _M_NONCOPYABLE_HPP_
#define _M_NONCOPYABLE_HPP_

class MNoncopyable
{
public:
    MNoncopyable(const MNoncopyable &) = delete;
    MNoncopyable& operator=(const MNoncopyable &) = delete;
};

#endif
