#ifndef _M_REDIS_COMMAND_H_
#define _M_REDIS_COMMAND_H_

#include <cherry/util/m_idb_command.h>
#include <string>
#include <vector>
#include <cherry/3rd/hiredis.h>

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
        *(static_cast<T*>(const_cast<char*>(str.c_str()))) = param;
        args_.push_back(param);
        return true;
    }
    template<typename T>
    bool GetBaseTypeParam(T &param)
    {
        if (!pp_result_)
        {
            return false;
        }
        if (cur_row_ >= row_count_)
        {
            return false;
        }
        redisReply *p_reply = pp_result_[cur_row_];
        if (!p_reply)
        {
            return false;
        }
        if (p_reply->type == REDIS_REPLY_INTEGER)
        {
            param = static_cast<T>(p_reply->integer);
        }
        else if (p_reply->type == REDIS_REPLY_STRING)
        {
            param = static_cast<T>(p_reply->str);
        }
        else
        {
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
