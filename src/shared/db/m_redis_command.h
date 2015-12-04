#ifndef _M_REDIS_COMMAND_H_
#define _M_REDIS_COMMAND_H_

#include <util/m_convert.h>
#include <util/m_type_define.h>
#include <db/m_idb_command.h>
#include <hiredis.h>
#include <string>
#include <vector>
#include <cstring>

class MRedisConnection;

class MRedisCommand
    :public MIDbCommand
{
public:
    explicit MRedisCommand(MRedisConnection &conn);
    virtual ~MRedisCommand();
private:
    virtual MDbError DoPrepair(const std::string &command) override;
    virtual MDbError DoBeforeAddParam() override;
    virtual MDbError DoGotoNextRecord() override;
    virtual MDbError DoGotoNextResult() override;
    virtual std::pair<unsigned, MDbError> DoExecuteNonQuery() override;
    virtual MDbError DoExecuteReader() override;

    virtual MDbError DoAddParam(const int8_t *p_param) override;
    virtual MDbError DoAddParam(const uint8_t *p_param) override;
    virtual MDbError DoAddParam(const int16_t *p_param) override;
    virtual MDbError DoAddParam(const uint16_t *p_param) override;
    virtual MDbError DoAddParam(const int32_t *p_param) override;
    virtual MDbError DoAddParam(const uint32_t *p_param) override;
    virtual MDbError DoAddParam(const int64_t *p_param) override;
    virtual MDbError DoAddParam(const uint64_t *p_param) override;
    virtual MDbError DoAddParam(const float *p_param) override;
    virtual MDbError DoAddParam(const double *p_param) override;
    virtual MDbError DoAddParam(const std::string *p_param) override;
    virtual MDbError DoAddParam(const MBlob *p_param) override;

    virtual MDbError DoGetParam(int8_t *p_param) override;
    virtual MDbError DoGetParam(uint8_t *p_param) override;
    virtual MDbError DoGetParam(int16_t *p_param) override;
    virtual MDbError DoGetParam(uint16_t *p_param) override;
    virtual MDbError DoGetParam(int32_t *p_param) override;
    virtual MDbError DoGetParam(uint32_t *p_param) override;
    virtual MDbError DoGetParam(int64_t *p_param) override;
    virtual MDbError DoGetParam(uint64_t *p_param) override;
    virtual MDbError DoGetParam(float *p_param) override;
    virtual MDbError DoGetParam(double *p_param) override;
    virtual MDbError DoGetParam(std::string *p_param) override;
    virtual MDbError DoGetParam(MBlob *p_param) override;
private:
    template<typename T>
    MDbError AddParamNumeric(const T *p_param)
    {
        in_params_.push_back(static_cast<const char*>(static_cast<const void*>(p_param)));
        in_param_lens_.push_back(sizeof(*p_param));
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return last_error_;
    }
    template<typename T>
    MDbError GetParamNumeric(T *p_param)
    {
        if (!pp_result_)
        {
            last_error_msg_ = "pp_result_ is null";
            last_error_ = MDbError::NoData;
            return last_error_;
        }
        if (cur_row_ >= row_count_)
        {
            last_error_msg_ = MConcat("cur row:", cur_row_, " is large than row count:", row_count_);
            last_error_ = MDbError::NoData;
            return last_error_;
        }
        redisReply *p_reply = pp_result_[cur_row_++];
        if (!p_reply)
        {
            last_error_msg_ = MConcat("p_reply is null, cur row:", cur_row_);
            last_error_ = MDbError::NoData;
            return last_error_;
        }
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        if (p_reply->type == REDIS_REPLY_INTEGER)
        {
            *p_param = static_cast<T>(p_reply->integer);
        }
        else if (p_reply->type == REDIS_REPLY_STRING)
        {
            std::string str(p_reply->str, p_reply->len);
            if (!MConvertTo(str, *p_param))
            {
                last_error_msg_ = "convert str to numeric failed";
                last_error_ = MDbError::ParamCannotConvert;
            }
        }
        else
        {
            last_error_msg_ = MConcat("type:", p_reply->type, " can't convert to numeric");
            last_error_ = MDbError::ParamCannotConvert;
        }
        return last_error_;
    }
private:
    MRedisConnection &conn_;
    redisReply *p_reply_;
    redisReply **pp_result_;
    std::string command_;
    std::vector<const char*> in_params_;
    std::vector<size_t> in_param_lens_;
    size_t cur_row_;
    size_t row_count_;
};

#endif
