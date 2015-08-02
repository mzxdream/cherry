#ifndef _M_ICLONEABLE_HPP_
#define _M_ICLONEABLE_HPP_

class MICloneable
{
public:
    virtual ~MICloneable() = default;
    virtual MICloneable* Clone() = 0;
};

#endif
