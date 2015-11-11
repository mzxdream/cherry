#ifndef _M_CONVERT_H_
#define _M_CONVERT_H_

#include <sstream>
#include <string>
#include <cherry/util/m_type_define.h>

class MConvert
{
public:
    template<typename T>
    static bool StrToBaseType(const std::string &from, T &to)
    {
        std::stringstream ss;
        return (ss << from) && (ss >> to);
    }
    template<typename >
    static bool BaseTypeToStr(const T &from, std::string &to)
    {
        std::stringstream ss;
        return (ss << from) && (ss >> to);
    }
};

#endif
