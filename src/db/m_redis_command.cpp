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

bool MRedisCommand::DoPrepair(const std::string &command)
{
    args_.resize(1);
    MBlob &arg = args_.back();
    arg.Resize(command.size());
    memcpy(arg.GetData(), command.c_str(), arg.GetSize());
    return true;
}

bool MRedisCommand::DoBeforeAddParam()
{
    args_.resize(1);
    return true;
}

int MRedisCommand::DoExecuteNonQuery()
{
    if (p_reply_)
    {
        freeReplyObject(p_reply_);
        p_reply_ = nullptr;
    }
    pp_result_ = nullptr;
    cur_row_ = 0;
    row_count_ = 0;

    std::vector<const char*> args(args_.size());
    std::vector<size_t> arglens(args_.size());

    for (size_t i = 0; i < args_.size(); ++i)
    {
        args[i] = args_[i].GetData();
        arglens[i] = args_[i].GetSize();
    }

    p_reply_ = static_cast<redisReply*>(redisCommandArgv(conn_.GetConnection(), args_.size(), &args[0], &arglens[0]));

    if (!p_reply_)
    {
        MLOG(Error) << "DoExecute failed reply is null";
        return false;
    }
    if (p_reply_->type == REDIS_REPLY_ERROR)
    {
        MLOG(Error) << "DoExecute failed reply error:" << p_reply_->str;
        return false;
    }
    else if (p_reply_->type == REDIS_REPLY_NIL)
    {
        return true;
    }
    else if (p_reply_->type == REDIS_REPLY_STATUS)
    {
        if (MString::CompareNoCase(p_reply_->str, "OK") != 0)
        {
            MLOG(Error) << "status is failed error:" << p_reply_->str;
            return false;
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

    int ret = 0;
    if (!NextRecord(ret))
    {
        MLOG(Error) << "get ret failed";
        return 0;
    }
    return ret;
}

bool MRedisCommand::DoExecuteReader()
{
    if (p_reply_)
    {
        freeReplyObject(p_reply_);
        p_reply_ = nullptr;
    }
    pp_result_ = nullptr;
    cur_row_ = 0;
    row_count_ = 0;

    std::vector<const char*> args(args_.size());
    std::vector<size_t> arglens(args_.size());

    for (size_t i = 0; i < args_.size(); ++i)
    {
        args[i] = args_[i].GetData();
        arglens[i] = args_[i].GetSize();
    }

    p_reply_ = static_cast<redisReply*>(redisCommandArgv(conn_.GetConnection(), args_.size(), &args[0], &arglens[0]));

    if (!p_reply_)
    {
        MLOG(Error) << "DoExecute failed reply is null";
        return false;
    }
    if (p_reply_->type == REDIS_REPLY_ERROR)
    {
        MLOG(Error) << "DoExecute failed reply error:" << p_reply_->str;
        return false;
    }
    else if (p_reply_->type == REDIS_REPLY_NIL)
    {
        return true;
    }
    else if (p_reply_->type == REDIS_REPLY_STATUS)
    {
        if (MString::CompareNoCase(p_reply_->str, "OK") != 0)
        {
            MLOG(Error) << "status is failed error:" << p_reply_->str;
            return false;
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
    return true;
}

bool MRedisCommand::DoAddParam(const int8_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const uint8_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const int16_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const uint16_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const int32_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const uint32_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const int64_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const uint64_t &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const float &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const double &param)
{
    return AddBaseTypeParam(param);
}

bool MRedisCommand::DoAddParam(const std::string &param)
{
    args_.resize(args_.size()+1);
    MBlob &arg = args_.back();
    arg.Resize(param.size());
    memcpy(arg.GetData(), param.c_str(), arg.GetSize());
    return true;
}

bool MRedisCommand::DoAddParam(const MBlob &param)
{
    args_.push_back(param);
    return true;
}

bool MRedisCommand::DoGetParam(int8_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(uint8_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(int16_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(uint16_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(int32_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(uint32_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(int64_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(uint64_t &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(float &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(double &param)
{
    return GetBaseTypeParam(param);
}

bool MRedisCommand::DoGetParam(std::string &param)
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
        if (!MConvert::BaseTypeToStr(p_reply->integer, param))
        {
            MLOG(Error) << "convert base type to str failed";
            return false;
        }
    }
    else if (p_reply->type == REDIS_REPLY_STRING)
    {
        param.resize(p_reply->len);
        memcpy(&param[0], p_reply->str, param.size());
    }
    else
    {
        MLOG(Error) << "type:" << p_reply->type << " can't get param";
        return false;
    }
    ++cur_row_;
    return true;
}

bool MRedisCommand::DoGetParam(MBlob &param)
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
        MLOG(Error) << "convert base type to blob";
        return false;
    }
    else if (p_reply->type == REDIS_REPLY_STRING)
    {
        param.Resize(p_reply->len);
        memcpy(param.GetData(), p_reply->str, param.GetSize());
    }
    else
    {
        MLOG(Error) << "type:" << p_reply->type << " can't get param";
        return false;
    }
    ++cur_row_;
    return true;
}
