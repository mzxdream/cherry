#ifndef _M_REDIS_COMMAND_H_
#define _M_REDIS_COMMAND_H_

#include <mzx/util/m_log.h>
#include <mzx/db/m_idb_command.h>
#include <mzx/util/m_convert.h>
#include <mzx/3rd/redis/hiredis.h>
#include <string>
#include <vector>

class MRedisConnection;

class MRedisCommand
    :public MIDbCommand
{
public:
    explicit MRedisCommand(MRedisConnection &conn);
    virtual ~MRedisCommand();
private:
    template<typename T>
    bool AddBaseTypeParam(const T &param)
    {
        std::string str;
        str.resize(sizeof(param));
        *(static_cast<T*>(static_cast<void*>(const_cast<char*>(str.c_str())))) = param;
        args_.push_back(str);
        return true;
    }
    template<typename T>
    bool GetBaseTypeParam(T &param)
    {
        if (!pp_result_)
        {
            MLOG(Error) << "pp_result_ is null";
            return false;
        }
        if (cur_row_ >= row_count_)
        {
            MLOG(Error) << "cur row:" << cur_row_ << " is large than row count:" << row_count_;
            return false;
        }
        redisReply *p_reply = pp_result_[cur_row_];
        if (!p_reply)
        {
            MLOG(Error) << "p_reply is null, cur row:" << cur_row_;
            return false;
        }
        if (p_reply->type == REDIS_REPLY_INTEGER)
        {
            param = static_cast<T>(p_reply->integer);
        }
        else if (p_reply->type == REDIS_REPLY_STRING)
        {
            if (!MConvert::StrToBaseType(p_reply->str, param))
            {
                MLOG(Error) << "convert str to base type failed";
                return false;
            }
        }
        else
        {
            MLOG(Error) << "type:" << p_reply->type << " can't get param";
            return false;
        }
        ++cur_row_;
        return true;
    }
private:
    virtual bool DoPrepair(const std::string &command) override;
    virtual bool DoBeforeAddParam() override;
    virtual int DoExecuteNonQuery() override;
    virtual bool DoExecuteReader() override;

    virtual bool DoAddParam(const int8_t &param) override;
    virtual bool DoAddParam(const uint8_t &param) override;
    virtual bool DoAddParam(const int16_t &param) override;
    virtual bool DoAddParam(const uint16_t &param) override;
    virtual bool DoAddParam(const int32_t &param) override;
    virtual bool DoAddParam(const uint32_t &param) override;
    virtual bool DoAddParam(const int64_t &param) override;
    virtual bool DoAddParam(const uint64_t &param) override;
    virtual bool DoAddParam(const float &param) override;
    virtual bool DoAddParam(const double &param) override;
    virtual bool DoAddParam(const std::string &param) override;
    virtual bool DoAddParam(const MBlob &param) override;

    virtual bool DoGetParam(int8_t &param) override;
    virtual bool DoGetParam(uint8_t &param) override;
    virtual bool DoGetParam(int16_t &param) override;
    virtual bool DoGetParam(uint16_t &param) override;
    virtual bool DoGetParam(int32_t &param) override;
    virtual bool DoGetParam(uint32_t &param) override;
    virtual bool DoGetParam(int64_t &param) override;
    virtual bool DoGetParam(uint64_t &param) override;
    virtual bool DoGetParam(float &param) override;
    virtual bool DoGetParam(double &param) override;
    virtual bool DoGetParam(std::string &param) override;
private:
    MRedisConnection &conn_;
    std::vector<std::string> args_;
    redisReply *p_reply_;
    redisReply **pp_result_;
    size_t cur_row_;
    size_t row_count_;
};

#endif
