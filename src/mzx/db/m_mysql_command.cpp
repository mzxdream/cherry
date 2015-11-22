#include <mzx/db/m_mysql_connection.h>
#include <mzx/db/m_mysql_command.h>
#include <cstring>

MMysqlCommand::MMysqlCommand(MMysqlConnection &conn)
    :conn_(conn)
    ,p_stmt_(nullptr)
    ,cur_row_(0)
    ,cur_col_(0)
{
}

MMysqlCommand::~MMysqlCommand()
{
    if (p_stmt_)
    {
        mysql_stmt_close(p_stmt_);
        p_stmt_ = nullptr;
    }
}

bool MMysqlCommand::DoPrepair(const std::string &command)
{
    if (p_stmt_)
    {
        mysql_stmt_close(p_stmt_);
        p_stmt_ = nullptr;
    }
    p_stmt_ = mysql_stmt_init(conn_.GetConnection());
    if (!p_stmt_)
    {
        MLOG(Error) << "init stmt failed errorno:" << mysql_errno(conn_.GetConnection()) << "error:" << mysql_error(conn_.GetConnection());
        return false;
    }
    if (mysql_stmt_prepare(p_stmt_, command.c_str(), command.size()) != 0)
    {
        MLOG(Error) << "mysql prepare " << command << " failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    return true;
}

bool MMysqlCommand::DoBeforeAddParam()
{
    params_.clear();
    datas_.clear();
    return true;
}

int MMysqlCommand::DoExecuteNonQuery()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (static_cast<size_t>(param_count) != params_.size())
    {
        MLOG(Error) << "param count is not match need:" << param_count << " actual is :" << params_.size();
        return 0;
    }
    if (mysql_stmt_bind_param(p_stmt_, &params_[0]) != 0)
    {
        MLOG(Error) << "bind failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        MLOG(Error) << "execute failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }
    return static_cast<int>(mysql_stmt_affected_rows(p_stmt_));
}

bool MMysqlCommand::DoExecuteReader()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (static_cast<size_t>(param_count) != params_.size())
    {
        MLOG(Error) << "param count is not match need:" << param_count << " actual is :" << params_.size();
        return false;
    }
    if (mysql_stmt_bind_param(p_stmt_, &params_[0]) != 0)
    {
        MLOG(Error) << "bind failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    my_bool update_max_length = 1;
    mysql_stmt_attr_set(p_stmt_, STMT_ATTR_UPDATE_MAX_LENGTH, static_cast<void*>(&update_max_length));
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        MLOG(Error) << "execute failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    return BindResult();
}

bool MMysqlCommand::DoAddParam(const int8_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_TINY, 0, param);
}

bool MMysqlCommand::DoAddParam(const uint8_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_TINY, 1, param);
}

bool MMysqlCommand::DoAddParam(const int16_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_SHORT, 0, param);
}

bool MMysqlCommand::DoAddParam(const uint16_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_SHORT, 1, param);
}

bool MMysqlCommand::DoAddParam(const int32_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_LONG, 0, param);
}

bool MMysqlCommand::DoAddParam(const uint32_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_LONG, 1, param);
}

bool MMysqlCommand::DoAddParam(const int64_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_LONGLONG, 0, param);
}

bool MMysqlCommand::DoAddParam(const uint64_t &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_LONGLONG, 1, param);
}

bool MMysqlCommand::DoAddParam(const float &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_FLOAT, 0, param);
}

bool MMysqlCommand::DoAddParam(const double &param)
{
    return AddBaseTypeParam(MYSQL_TYPE_DOUBLE, 0, param);
}

bool MMysqlCommand::DoAddParam(const std::string &param)
{
    datas_.push_back(param);
    std::string &str = datas_.back();
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_VAR_STRING;
    bind.buffer = const_cast<char*>(str.c_str());
    bind.buffer_length = str.size();
    bind.length = nullptr;
    bind.is_null = nullptr;
    bind.is_unsigned = 0;
    params_.push_back(bind);
    return true;
}

bool MMysqlCommand::DoAddParam(const MBlob &param)
{
    datas_.push_back(param.GetString());
    std::string &str = datas_.back();
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_BLOB;
    bind.buffer = const_cast<char*>(str.c_str());
    bind.buffer_length = str.size();
    bind.length = nullptr;
    bind.is_null = nullptr;
    bind.is_unsigned = 0;
    params_.push_back(bind);
    return true;
}

bool MMysqlCommand::DoGetParam(int8_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(uint8_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(int16_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(uint16_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(int32_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(uint32_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(int64_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(uint64_t &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(float &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(double &param)
{
    return CheckResult() && GetBaseTypeParam(param);
}

bool MMysqlCommand::DoGetParam(std::string &param)
{
    if (!CheckResult())
    {
        return false;
    }
    if (cur_col_ >= out_params_.size())
    {
        return false;
    }
    const MYSQL_BIND &bind = out_params_[cur_col_++];
    if (*(bind.is_null))
    {
        param = "";
        return true;
    }
    if (bind.buffer_type == MYSQL_TYPE_NULL)
    {
        param = "";
        return true;
    }
    if (bind.buffer_type == MYSQL_TYPE_TINY_BLOB
        || bind.buffer_type == MYSQL_TYPE_MEDIUM_BLOB
        || bind.buffer_type == MYSQL_TYPE_LONG_BLOB
        || bind.buffer_type == MYSQL_TYPE_BLOB
        || bind.buffer_type == MYSQL_TYPE_STRING
        || bind.buffer_type == MYSQL_TYPE_VAR_STRING)
    {
        param.resize(*(bind.length));
        memcpy(const_cast<char*>(param.c_str()), bind.buffer, param.size());
        return true;
    }
    MLOG(Error) << "can't convert type:" << bind.buffer_type << " to string";
    return false;
}

bool MMysqlCommand::BindResult()
{
    if (mysql_stmt_store_result(p_stmt_) != 0)
    {
        MLOG(Error) << "store result failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    MYSQL_RES *p_meta_res = mysql_stmt_result_metadata(p_stmt_);
    if (!p_meta_res)
    {
        MLOG(Error) << "have not meta res errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    bool ret = false;
    do
    {
        MYSQL_FIELD *p_field_list = mysql_fetch_fields(p_meta_res);
        if (!p_field_list)
        {
            MLOG(Error) << "mysql_fetch_fields is null unknown error";
            break;
        }
        unsigned long field_count = mysql_num_fields(p_meta_res);
        if (field_count == 0)
        {
            MLOG(Error) << "mysql field_count is 0";
            break;
        }
        out_params_.resize(field_count);
        out_datas_.resize(field_count);
        out_lengths_.resize(field_count);
        out_is_nulls_.resize(field_count);
        for (size_t i = 0; i < field_count; ++i)
        {
            out_datas_[i].resize(p_field_list[i].max_length);
            memset(&(out_params_[i]), 0, sizeof(out_params_[i]));
            out_params_[i].buffer_type = p_field_list[i].type;
            out_params_[i].buffer = const_cast<char*>(out_datas_[i].c_str());
            out_params_[i].buffer_length = p_field_list[i].max_length;
            out_params_[i].length = &(out_lengths_[i]);
            out_params_[i].is_null = &(out_is_nulls_[i]);
            out_params_[i].is_unsigned = (p_field_list[i].flags & UNSIGNED_FLAG);
            out_params_[i].error = nullptr;
        }
        if (mysql_stmt_bind_result(p_stmt_, &out_params_[0]) != 0)
        {
            MLOG(Error) << "bind result failed errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
            break;
        }
        cur_row_ = 0;
        cur_col_ = 0;
        ret = true;
    } while (0);
    mysql_free_result(p_meta_res);
    return ret;
}

bool MMysqlCommand::FetchNextResult()
{
    int ret = mysql_stmt_next_result(p_stmt_);
    if (ret == 0)
    {
        return BindResult();
    }
    else if (ret < 0)
    {
        return false;
    }
    else
    {
        MLOG(Error) << "failed to get next result errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
}

bool MMysqlCommand::FetchNextRow()
{
    int ret = mysql_stmt_fetch(p_stmt_);
    if (ret == 0)
    {
        return true;
    }
    else if (ret == MYSQL_NO_DATA)
    {
        FetchNextResult();
        return false;
    }
    else
    {
        MLOG(Error) << "fetch errorno:" << mysql_stmt_errno(p_stmt_) << " error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
}

bool MMysqlCommand::CheckResult()
{
    if (cur_row_ == 0
        || cur_col_ >= out_params_.size())
    {
        if (!FetchNextRow())
        {
            return false;
        }
        ++cur_row_;
        cur_col_ = 0;
    }
    return true;
}

