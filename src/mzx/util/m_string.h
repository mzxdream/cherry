#ifndef _M_STRING_H_
#define _M_STRING_H_

#include <string>
#include <strings.h>

class MString
{
public:
    static int CompareNoCase(const std::string &s1, const std::string &s2)
    {
        return strcasecmp(s1.c_str(), s2.c_str());
    }
};

#endif
