#include <db/m_redis_connection.h>
#include <db/m_redis_command.h>
#include <util/m_string.h>

MRedisCommand::MRedisCommand(MRedisConnection &conn)
    :conn_(conn)
    ,p_reply_(nullptr)
    ,pp_result_(nullptr)
    ,cur_row_(0)
    ,row_count_(0)
{
}

MRedisCommand::~MRedisCommand()
{
    if (p_reply_)
    {
        freeReplyObject(p_reply_);
        p_reply_ = nullptr;
    }
    pp_result_ = nullptr;
    cur_row_ = 0;
    row_count_ = 0;
}

MDbError MRedisCommand::DoPrepair(const std::string &command)
{
    command_ = command;
    in_params_.clear();
    in_param_lens_.clear();
    in_params_.push_back(&command_[0]);
    in_param_lens_.push_back(command_.size());
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoBeforeAddParam()
{
    in_params_.resize(1);
    in_param_lens_.resize(1);
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoGotoNextRecord()
{
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoGotoNextResult()
{
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

std::pair<unsigned, MDbError> MRedisCommand::DoExecuteNonQuery()
{
    if (p_reply_)
    {
        freeReplyObject(p_reply_);
        p_reply_ = nullptr;
    }
    pp_result_ = nullptr;
    cur_row_ = 0;
    row_count_ = 0;

    p_reply_ = static_cast<redisReply*>(redisCommandArgv(conn_.GetConnection(), in_params_.size(), &in_params_[0], &in_param_lens_[0]));

    if (!p_reply_)
    {
        last_error_msg_ = "Execute failed reply is null";
        last_error_ = MDbError::Unknown;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    if (p_reply_->type == REDIS_REPLY_ERROR)
    {
        last_error_msg_ = MConcat("Execute failed reply error:", p_reply_->str);
        last_error_ = MDbError::Unknown;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    else if (p_reply_->type == REDIS_REPLY_NIL)
    {
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    else if (p_reply_->type == REDIS_REPLY_STATUS)
    {
        if (MString::CompareNoCase(p_reply_->str, "OK") != 0)
        {
            last_error_msg_ = MConcat("status is failed error:", p_reply_->str);
            last_error_ = MDbError::Unknown;
            return std::make_pair(static_cast<unsigned>(0), last_error_);
        }
    }
    else if (p_reply_->type == REDIS_REPLY_INTEGER
        || p_reply_->type == REDIS_REPLY_STRING)
    {
        pp_result_ = &p_reply_;
        row_count_ = 1;
    }
    else if (p_reply_->type == REDIS_REPLY_ARRAY)
    {
        pp_result_ = p_reply_->element;
        row_count_ = p_reply_->elements;
    }

    unsigned ret = 0;
    last_error_ = NextRecord(&ret);
    return std::make_pair(ret, last_error_);
}

MDbError MRedisCommand::DoExecuteReader()
{
    if (p_reply_)
    {
        freeReplyObject(p_reply_);
        p_reply_ = nullptr;
    }
    pp_result_ = nullptr;
    cur_row_ = 0;
    row_count_ = 0;

    p_reply_ = static_cast<redisReply*>(redisCommandArgv(conn_.GetConnection(), in_params_.size(), &in_params_[0], &in_param_lens_[0]));

    if (!p_reply_)
    {
        last_error_msg_ = "Execute failed reply is null";
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    if (p_reply_->type == REDIS_REPLY_ERROR)
    {
        last_error_msg_ = MConcat("Execute failed reply error:", p_reply_->str);
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    else if (p_reply_->type == REDIS_REPLY_NIL)
    {
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return last_error_;
    }
    else if (p_reply_->type == REDIS_REPLY_STATUS)
    {
        if (MString::CompareNoCase(p_reply_->str, "OK") != 0)
        {
            last_error_msg_ = MConcat("status is failed error:", p_reply_->str);
            last_error_ = MDbError::Unknown;
            return last_error_;
        }
    }
    else if (p_reply_->type == REDIS_REPLY_INTEGER
        || p_reply_->type == REDIS_REPLY_STRING)
    {
        pp_result_ = &p_reply_;
        row_count_ = 1;
    }
    else if (p_reply_->type == REDIS_REPLY_ARRAY)
    {
        pp_result_ = p_reply_->element;
        row_count_ = p_reply_->elements;
    }
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoAddParam(const int8_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const uint8_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const int16_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const uint16_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const int32_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const uint32_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const int64_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const uint64_t *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const float *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const double *p_param)
{
    return AddParamNumeric(p_param);
}

MDbError MRedisCommand::DoAddParam(const std::string *p_param)
{
    in_params_.push_back(&((*p_param)[0]));
    in_param_lens_.push_back(p_param->size());
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoAddParam(const MBlob *p_param)
{
    in_params_.push_back(p_param->GetData());
    in_param_lens_.push_back(p_param->GetSize());
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MRedisCommand::DoGetParam(int8_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(uint8_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(int16_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(uint16_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(int32_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(uint32_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(int64_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(uint64_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(float *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(double *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MRedisCommand::DoGetParam(std::string *p_param)
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
        if (!MConvertTo(p_reply->integer, *p_param))
        {
            last_error_msg_ = "convert int to string failed";
            last_error_ = MDbError::ParamCannotConvert;
        }
    }
    else if (p_reply->type == REDIS_REPLY_STRING)
    {
        p_param->resize(p_reply->len);
        memcpy(&((*p_param)[0]), p_reply->str, p_param->size());
    }
    else
    {
        last_error_msg_ = MConcat("type:", p_reply->type, " can't convert to string");
        last_error_ = MDbError::ParamCannotConvert;
    }
    return last_error_;
}

MDbError MRedisCommand::DoGetParam(MBlob *p_param)
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
        last_error_msg_ = "convert int to blob failed";
        last_error_ = MDbError::ParamCannotConvert;
    }
    else if (p_reply->type == REDIS_REPLY_STRING)
    {
        p_param->Resize(p_reply->len);
        memcpy(p_param->GetData(), p_reply->str, p_param->GetSize());
    }
    else
    {
        last_error_msg_ = MConcat("type:", p_reply->type, " can't convert to blob");
        last_error_ = MDbError::ParamCannotConvert;
    }
    return last_error_;
}
