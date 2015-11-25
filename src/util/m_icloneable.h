#ifndef _M_ICLONEABLE_H_
#define _M_ICLONEABLE_H_

class MICloneable
{
public:
    virtual ~MICloneable() = default;
    virtual MICloneable* Clone() = 0;
};

#endif
