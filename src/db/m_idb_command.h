#ifndef _M_IDB_COMMAND_H_
#define _M_IDB_COMMAND_H_

#include <string>
#include <util/m_type_define.h>
#include <util/m_log.h>

class MIDbCommand
{
public:
    MIDbCommand() = default;
    virtual ~MIDbCommand() = default;
    MIDbCommand(const MIDbCommand &) = delete;
    MIDbCommand& operator=(const MIDbCommand &) = delete;
public:
    bool Prepair(const std::string &command)
    {
        return DoPrepair(command);
    }
    template<typename... Args>
    int ExecuteNonQuery(const Args&... args)
    {
        if (!DoBeforeAddParam())
        {
            MLOG(Error) << "ExecuteNonQuery DoBeforeAddParam failed";
            return false;
        }
        if (!DoAddParam(args...))
        {
            MLOG(Error) << "ExecuteNonQuery DoAddParam failed";
            return false;
        }
        return DoExecuteNonQuery();
    }

    template<typename... Args>
    bool ExecuteReader(const Args&... args)
    {
        if (!DoBeforeAddParam())
        {
            MLOG(Error) << "ExecuteReader DoBeforeAddParam failed";
            return false;
        }
        if (!DoAddParam(args...))
        {
            MLOG(Error) << "ExecuteReader DoAddParam failed";
            return false;
        }
        return DoExecuteReader();
    }
    template<typename... Args>
    bool NextRecord(Args&... args)
    {
        if (!DoBeforeGetParam())
        {
            MLOG(Error) << "do before get param failed";
            return false;
        }
        return DoGetParam(args...);
    }
private:
    virtual bool DoPrepair(const std::string &command) = 0;
    virtual bool DoBeforeAddParam() { return true; }
    virtual bool DoBeforeGetParam() { return true; }
    virtual int DoExecuteNonQuery() = 0;
    virtual bool DoExecuteReader() = 0;

    virtual bool DoAddParam(const int8_t &param) { return false; }
    virtual bool DoAddParam(const uint8_t &param) { return false; }
    virtual bool DoAddParam(const int16_t &param) { return false; }
    virtual bool DoAddParam(const uint16_t &param) { return false; }
    virtual bool DoAddParam(const int32_t &param) { return false; }
    virtual bool DoAddParam(const uint32_t &param) { return false; }
    virtual bool DoAddParam(const int64_t &param) { return false; }
    virtual bool DoAddParam(const uint64_t &param) { return false; }
    virtual bool DoAddParam(const float &param) { return false; }
    virtual bool DoAddParam(const double &param) { return false; }
    virtual bool DoAddParam(const std::string &param) { return false; }
    virtual bool DoAddParam(const MBlob &param) { return false; }

    virtual bool DoGetParam(int8_t &param) { return false; }
    virtual bool DoGetParam(uint8_t &param) { return false; }
    virtual bool DoGetParam(int16_t &param) { return false; }
    virtual bool DoGetParam(uint16_t &param) { return false; }
    virtual bool DoGetParam(int32_t &param) { return false; }
    virtual bool DoGetParam(uint32_t &param) { return false; }
    virtual bool DoGetParam(int64_t &param) { return false; }
    virtual bool DoGetParam(uint64_t &param) { return false; }
    virtual bool DoGetParam(float &param) { return false; }
    virtual bool DoGetParam(double &param) { return false; }
    virtual bool DoGetParam(std::string &param) { return false; }
    virtual bool DoGetParam(MBlob &param){ return false; }
private:
    template<typename T>
    bool DoAddParam(const T &param)
    {
        MLOG(Error) << "DoAddParam param type is not support";
        return false;
    }
    template<typename T, typename... Args>
    bool DoAddParam(const T &param, const Args&... args)
    {
        return DoAddParam(param) && DoAddParam(args...);
    }
    template<typename T>
    bool DoGetParam(T &param)
    {
        MLOG(Error) << "DoGetParam param type is not support";
        return false;
    }
    template<typename T, typename... Args>
    bool DoGetParam(T &param, Args&... args)
    {
        return DoGetParam(param) && DoGetParam(args...);
    }
};

#endif
