#ifndef _M_IDB_COMMAND_H_
#define _M_IDB_COMMAND_H_

#include <string>
#include <util/m_type_define.h>
#include <db/m_db_common.h>
#include <utility>

class MIDbCommand
{
public:
    MIDbCommand() = default;
    virtual ~MIDbCommand() = default;
    MIDbCommand(const MIDbCommand &) = delete;
    MIDbCommand& operator=(const MIDbCommand &) = delete;
public:
    MDbError Prepair(const std::string &command)
    {
        return DoPrepair(command);
    }
    template<typename... Args>
    std::pair<unsigned, MDbError> ExecuteNonQuery(const Args&... args)
    {
        unsigned affect = 0;
        MDbError ret = DoBeforeAddParam();
        if (ret == MDbError::No)
        {
            ret = DoAddParam(args...);
            if (ret == MDbError::No)
            {
                return DoExecuteNonQuery();
            }
        }
        return std::make_pair(affect, ret);
    }
    template<typename... Args>
    MDbError ExecuteReader(const Args&... args)
    {
        MDbError ret = DoBeforeAddParam();
        if (ret == MDbError::No)
        {
            ret = DoAddParam(args...);
            if (ret == MDbError::No)
            {
                return DoExecuteReader();
            }
        }
        return ret;
    }
    template<typename... Args>
    MDbError NextRecord(Args&... args)
    {
        MDbError ret = DoGotoNextRecord();
        if (ret == MDbError::No)
        {
            return DoGetParam(args...);
        }
        return ret;
    }
    MDbError GotoNextResult()
    {
        return DoGotoNextResult();
    }
private:
    virtual MDbError DoPrepair(const std::string &command) = 0;
    virtual MDbError DoBeforeAddParam() = 0;
    virtual MDbError DoGotoNextRecord() = 0;
    virtual MDbError DoGotoNextResult() = 0;
    virtual std::pair<unsigned, MDbError> DoExecuteNonQuery() = 0;
    virtual MDbError DoExecuteReader() = 0;

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
