#define _M_TYPE_DEFINE_H_
#define _M_TYPE_DEFINE_H_

#include <stdint.h>
#include <string>

class MBlob final
{
public:
    explicit MBlob(const std::string &data)
        :data_(data)
    {
    }
    ~MBlob() = default;
    MBlob(const MBlob &) = default;
    MBlob& operator=(const MBlob &) = default;
private:
    std::string data_;
};

#endif
