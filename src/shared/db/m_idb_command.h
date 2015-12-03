#ifndef _M_IDB_COMMAND_H_
#define _M_IDB_COMMAND_H_

#include <string>
#include <util/m_type_define.h>
#include <db/m_db_common.h>
#include <utility>

class MIDbCommand
{
public:
    MIDbCommand()
        :last_error_(MDbError::No)
    {
    }
    virtual ~MIDbCommand() = default;
    MIDbCommand(const MIDbCommand &) = delete;
    MIDbCommand& operator=(const MIDbCommand &) = delete;
public:
    MDbError Prepair(const std::string &command) { return DoPrepair(command); }
    template<typename... Args>
    std::pair<unsigned, MDbError> ExecuteNonQuery(const Args&... args)
    {
        MDbError ret = DoBeforeAddParam();
        if (ret == MDbError::No)
        {
            ret = AddParams(args...);
            if (ret == MDbError::No)
            {
                return DoExecuteNonQuery();
            }
        }
        return std::make_pair(static_cast<unsigned>(0), ret);
    }
    template<typename... Args>
    MDbError ExecuteReader(const Args&... args)
    {
        MDbError ret = DoBeforeAddParam();
        if (ret == MDbError::No)
        {
            ret = AddParams(args...);
            if (ret == MDbError::No)
            {
                return DoExecuteReader();
            }
        }
        return ret;
    }
    template<typename... Args>
    MDbError NextRecord(Args*... args)
    {
        MDbError ret = DoGotoNextRecord();
        if (ret == MDbError::No)
        {
            return GetParams(args...);
        }
        return ret;
    }
    MDbError NextResult() { return DoGotoNextResult(); }
    MDbError GetLastError() const { return last_error_; }
    const std::string& GetLastErrorMsg() const { return last_error_msg_; }
private:
    virtual MDbError DoPrepair(const std::string &command) = 0;
    virtual MDbError DoBeforeAddParam() = 0;
    virtual MDbError DoGotoNextRecord() = 0;
    virtual MDbError DoGotoNextResult() = 0;
    virtual std::pair<unsigned, MDbError> DoExecuteNonQuery() = 0;
    virtual MDbError DoExecuteReader() = 0;

    virtual MDbError DoAddParam(const int8_t *p_param) = 0;
    virtual MDbError DoAddParam(const uint8_t *p_param) = 0;
    virtual MDbError DoAddParam(const int16_t *p_param) = 0;
    virtual MDbError DoAddParam(const uint16_t *p_param) = 0;
    virtual MDbError DoAddParam(const int32_t *p_param) = 0;
    virtual MDbError DoAddParam(const uint32_t *p_param) = 0;
    virtual MDbError DoAddParam(const int64_t *p_param) = 0;
    virtual MDbError DoAddParam(const uint64_t *p_param) = 0;
    virtual MDbError DoAddParam(const float *p_param) = 0;
    virtual MDbError DoAddParam(const double *p_param) = 0;
    virtual MDbError DoAddParam(const std::string *p_param) = 0;
    virtual MDbError DoAddParam(const MBlob *p_param) = 0;

    virtual MDbError DoGetParam(int8_t *p_param) = 0;
    virtual MDbError DoGetParam(uint8_t *p_param) = 0;
    virtual MDbError DoGetParam(int16_t *p_param) = 0;
    virtual MDbError DoGetParam(uint16_t *p_param) = 0;
    virtual MDbError DoGetParam(int32_t *p_param) = 0;
    virtual MDbError DoGetParam(uint32_t *p_param) = 0;
    virtual MDbError DoGetParam(int64_t *p_param) = 0;
    virtual MDbError DoGetParam(uint64_t *p_param) = 0;
    virtual MDbError DoGetParam(float *p_param) = 0;
    virtual MDbError DoGetParam(double *p_param) = 0;
    virtual MDbError DoGetParam(std::string *p_param) = 0;
    virtual MDbError DoGetParam(MBlob *p_param) = 0;
private:
    MDbError AddParams() { return MDbError::No; }
    template<typename T, typename... Args>
    MDbError AddParams(const T &param, const Args&... args)
    {
        return DoAddParam(&param) && AddParams(args...);
    }
    MDbError GetParams() { return MDbError::No; }
    template<typename T, typename... Args>
    MDbError GetParams(T &param, Args&... args)
    {
        return DoGetParam(param) && GetParams(args...);
    }
protected:
    MDbError last_error_;
    std::string last_error_msg_;
};

#endif
