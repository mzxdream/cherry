#ifndef _M_IDB_COMMAND_H_
#define _M_IDB_COMMAND_H_

#include <string>
#include <cherry/util/m_type_define.h>
#include <cherry/util/m_log.h>

class MIDbConnection;

class MIDbCommand
{
public:
    explicit MIDbCommand(MIDbConnection &conn)
        :conn_(conn)
    {
    }
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
            return -1;
        }
        if (!AddParam(args...))
        {
            MLOG(Error) << "ExecuteNonQuery AddParam failed";
            return -2;
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
        if (!AddParam(args...))
        {
            MLOG(Error) << "ExecuteReader AddParam failed";
            return false;
        }
        return DoExecuteReader();
    }
    template<typename... Args>
    bool NextRecord(Args&... args)
    {
        return GetParam(args...);
    }
private:
    virtual bool DoPrepair(const std::string &command) = 0;
    virtual bool DoBeforeAddParam() = 0;
    virtual int DoExecuteNonQuery() = 0;
    virtual bool DoExecuteReader() = 0;

    virtual bool AddParam(const int8_t &param) { return false; }
    virtual bool AddParam(const uint8_t &param) { return false; }
    virtual bool AddParam(const int16_t &param) { return false; }
    virtual bool AddParam(const uint16_t &param) { return false; }
    virtual bool AddParam(const int32_t &param) { return false; }
    virtual bool AddParam(const uint32_t &param) { return false; }
    virtual bool AddParam(const int64_t &param) { return false; }
    virtual bool AddParam(const uint64_t &param) { return false; }
    virtual bool AddParam(const float &param) { return false; }
    virtual bool AddParam(const double &param) { return false; }
    virtual bool AddParam(const std::string &param) { return false; }
    virtual bool AddParam(const MBlob &param) { return false; }

    virtual bool GetParam(int8_t &param) { return false; }
    virtual bool GetParam(uint8_t &param) { return false; }
    virtual bool GetParam(int16_t &param) { return false; }
    virtual bool GetParam(uint16_t &param) { return false; }
    virtual bool GetParam(int32_t &param) { return false; }
    virtual bool GetParam(uint32_t &param) { return false; }
    virtual bool GetParam(int64_t &param) { return false; }
    virtual bool GetParam(uint64_t &param) { return false; }
    virtual bool GetParam(float &param) { return false; }
    virtual bool GetParam(double &param) { return false; }
    virtual bool GetParam(std::string &param) { return false; }
private:
    template<typename T>
    bool AddParam(const T &param)
    {
        return false;
    }
    template<typename T, typename... Args>
    bool AddParam(const T &param, const Args&... args)
    {
        return AddParam(param) && AddParam(args...);
    }
    template<typename T>
    bool GetParam(T &param)
    {
        return false;
    }
    template<typename T, typename... Args>
    bool GetParam(T &param, Args&... args)
    {
        return GetParam(param) && GetParam(args...);
    }
private:
    MIDbConnection &conn_;
};

#endif
