#ifndef _M_STRING_H_
#define _M_STRING_H_

#include <string>
#include <strings.h>
#include <mzx/util/m_convert.h>

class MString
{
public:
    static int CompareNoCase(const std::string &s1, const std::string &s2)
    {
        return strcasecmp(s1.c_str(), s2.c_str());
    }
    static std::string TrimLeft(const std::string &str, const std::string &trim)
    {
        size_t pos = str.find_first_not_of(trim);
        if (pos != std::string::npos)
        {
            return str.substr(pos);
        }
        return "";
    }
    static std::string TrimRight(const std::string &str, const std::string &trim)
    {
        size_t pos = str.find_last_not_of(trim);
        if (pos != std::string::npos)
        {
            return str.substr(0, pos+1);
        }
        return "";
    }
    static std::string Trim(const std::string &str, const std::string &trim)
    {
        return TrimRight(TrimLeft(str, trim), trim);
    }
    template<typename T>
    static bool GetParamValue(const std::string &str, const std::string &param, T &val, const std::string &op="=", const std::string &sep=";", const std::string &trim=" ")
    {
        std::string val_str;
        if (!GetParamValue(str, param, val_str, op, sep, trim))
        {
            return false;
        }
        if (!MConvert::StrToBaseType(val_str, val))
        {
            return false;
        }
        return true;
    }
    static bool GetParamValue(const std::string &str, const std::string &param, std::string &val, const std::string &op="=", const std::string &sep=";", const std::string &trim=" ")
    {
        size_t pos_start = str.find(param);
        if (pos_start == std::string::npos)
        {
            return false;
        }
        pos_start += param.size();
        pos_start = str.find(op, pos_start);
        if (pos_start == std::string::npos)
        {
            return false;
        }
        pos_start += op.size();

        size_t pos_end = str.find(sep, pos_start);
        if (pos_end == std::string::npos)
        {
            val = Trim(str.substr(pos_start), trim);
        }
        else
        {
            val = Trim(str.substr(pos_start, pos_end-pos_start), trim);
        }
        return true;
    }
};

#endif
