#include <db/m_mysql_connection.h>
#include <db/m_mysql_command.h>
#include <cstring>

MMysqlCommand::MMysqlCommand(MMysqlConnection &conn)
    :conn_(conn)
    ,p_stmt_(nullptr)
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

MDbError MMysqlCommand::DoPrepair(const std::string &command)
{
    if (p_stmt_)
    {
        mysql_stmt_close(p_stmt_);
        p_stmt_ = nullptr;
    }
    p_stmt_ = mysql_stmt_init(conn_.GetConnection());
    if (!p_stmt_)
    {
        last_error_msg_ = MConcat("init stmt failed errorno:", mysql_errno(conn_.GetConnection()), "error:", mysql_error(conn_.GetConnection()));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    if (mysql_stmt_prepare(p_stmt_, command.c_str(), command.size()) != 0)
    {
        last_error_msg_ = MConcat("mysql prepare ", command, " failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MMysqlCommand::DoBeforeAddParam()
{
    in_params_.clear();
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MMysqlCommand::DoGotoNextRecord()
{
    int ret = mysql_stmt_fetch(p_stmt_);
    if (ret == 0)
    {
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        cur_col_ = 0;
    }
    else if (ret == MYSQL_NO_DATA)
    {
        last_error_msg_ = "have no more data";
        last_error_ = MDbError::NoData;
    }
    else
    {
        last_error_msg_ = MConcat("next record errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
    }
    return last_error_;
}

MDbError MMysqlCommand::DoGotoNextResult()
{
    int ret = mysql_stmt_next_result(p_stmt_);
    if (ret == 0)
    {
        return BindResult();
    }
    else if (ret < 0)
    {
        last_error_msg_ = MConcat("next result ret is ", ret);
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    else
    {
        last_error_msg_ =  MConcat("next result errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
}

std::pair<unsigned, MDbError> MMysqlCommand::DoExecuteNonQuery()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (static_cast<size_t>(param_count) != in_params_.size())
    {
        last_error_msg_ = MConcat("param count is not match need:", param_count, " actual is :", in_params_.size());
        last_error_ = MDbError::ParamCountNotMatch;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    if (mysql_stmt_bind_param(p_stmt_, &in_params_[0]) != 0)
    {
        last_error_msg_ = MConcat("bind failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        last_error_msg_ = MConcat("execute failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return std::make_pair(static_cast<unsigned>(0), last_error_);
    }
    unsigned affect = mysql_stmt_affected_rows(p_stmt_);
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return std::make_pair(affect, last_error_);
}

MDbError MMysqlCommand::DoExecuteReader()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (static_cast<size_t>(param_count) != in_params_.size())
    {
        last_error_msg_ = MConcat("param count is not match need:", param_count, " actual is :", in_params_.size());
        last_error_ = MDbError::ParamCountNotMatch;
        return last_error_;
    }
    if (mysql_stmt_bind_param(p_stmt_, &in_params_[0]) != 0)
    {
        last_error_msg_ = MConcat("bind failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    my_bool update_max_length = 1;
    mysql_stmt_attr_set(p_stmt_, STMT_ATTR_UPDATE_MAX_LENGTH, static_cast<void*>(&update_max_length));
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        last_error_msg_ = MConcat("execute failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    return BindResult();
}

MDbError MMysqlCommand::DoAddParam(const int8_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_TINY, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const uint8_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_TINY, 1, p_param);
}

MDbError MMysqlCommand::DoAddParam(const int16_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_SHORT, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const uint16_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_SHORT, 1, p_param);
}

MDbError MMysqlCommand::DoAddParam(const int32_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_LONG, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const uint32_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_LONG, 1, p_param);
}

MDbError MMysqlCommand::DoAddParam(const int64_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_LONGLONG, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const uint64_t *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_LONGLONG, 1, p_param);
}

MDbError MMysqlCommand::DoAddParam(const float *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_FLOAT, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const double *p_param)
{
    return AddParamNumeric(MYSQL_TYPE_DOUBLE, 0, p_param);
}

MDbError MMysqlCommand::DoAddParam(const std::string *p_param)
{
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_VAR_STRING;
    bind.buffer = const_cast<char*>(&((*p_param)[0]));
    bind.buffer_length = p_param->size();
    bind.length = nullptr;
    bind.is_null = nullptr;
    bind.is_unsigned = 0;
    in_params_.push_back(bind);
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MMysqlCommand::DoAddParam(const MBlob *p_param)
{
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_BLOB;
    bind.buffer = const_cast<char*>(p_param->GetData());
    bind.buffer_length = p_param->GetSize();
    bind.length = nullptr;
    bind.is_null = nullptr;
    bind.is_unsigned = 0;
    in_params_.push_back(bind);
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    return last_error_;
}

MDbError MMysqlCommand::DoGetParam(int8_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(uint8_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(int16_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(uint16_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(int32_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(uint32_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(int64_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(uint64_t *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(float *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(double *p_param)
{
    return GetParamNumeric(p_param);
}

MDbError MMysqlCommand::DoGetParam(std::string *p_param)
{
    if (cur_col_ >= out_params_.size())
    {
        last_error_msg_ = "record have no data";
        last_error_ = MDbError::NoData;
        return last_error_;
    }
    const MYSQL_BIND &bind = out_params_[cur_col_++];
    if (*(bind.is_null)
            || bind.buffer_type == MYSQL_TYPE_NULL)
    {
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        *p_param = "";
        return last_error_;
    }
    if (bind.buffer_type == MYSQL_TYPE_TINY_BLOB
        || bind.buffer_type == MYSQL_TYPE_MEDIUM_BLOB
        || bind.buffer_type == MYSQL_TYPE_LONG_BLOB
        || bind.buffer_type == MYSQL_TYPE_BLOB
        || bind.buffer_type == MYSQL_TYPE_STRING
        || bind.buffer_type == MYSQL_TYPE_VAR_STRING)
    {
        p_param->resize(*(bind.length));
        memcpy(&((*p_param)[0]), bind.buffer, p_param->size());
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return last_error_;
    }
    last_error_msg_ = MConcat("can't convert type:", bind.buffer_type, " to string");
    last_error_ = MDbError::ParamCannotConvert;
    return last_error_;
}

MDbError MMysqlCommand::DoGetParam(MBlob *p_param)
{
    if (cur_col_ >= out_params_.size())
    {
        last_error_msg_ = "record have no data";
        last_error_ = MDbError::NoData;
        return last_error_;
    }
    const MYSQL_BIND &bind = out_params_[cur_col_++];
    if (*(bind.is_null)
            || bind.buffer_type == MYSQL_TYPE_NULL)
    {
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        p_param->Resize(0);
        return last_error_;
    }
    if (bind.buffer_type == MYSQL_TYPE_TINY_BLOB
        || bind.buffer_type == MYSQL_TYPE_MEDIUM_BLOB
        || bind.buffer_type == MYSQL_TYPE_LONG_BLOB
        || bind.buffer_type == MYSQL_TYPE_BLOB
        || bind.buffer_type == MYSQL_TYPE_STRING
        || bind.buffer_type == MYSQL_TYPE_VAR_STRING)
    {
        p_param->Resize(*(bind.length));
        memcpy(p_param->GetData(), bind.buffer, p_param->GetSize());
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return last_error_;
    }
    last_error_msg_ = MConcat("can't convert type:", bind.buffer_type, " to MBlob");
    last_error_ = MDbError::ParamCannotConvert;
    return last_error_;
}

MDbError MMysqlCommand::BindResult()
{
    if (mysql_stmt_store_result(p_stmt_) != 0)
    {
        last_error_msg_ = MConcat("store result failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    MYSQL_RES *p_meta_res = mysql_stmt_result_metadata(p_stmt_);
    if (!p_meta_res)
    {
        last_error_msg_ = MConcat("have not meta res errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    do
    {
        MYSQL_FIELD *p_field_list = mysql_fetch_fields(p_meta_res);
        if (!p_field_list)
        {
           last_error_msg_ = "mysql_fetch_fields is null unknown error";
           last_error_ = MDbError::Unknown;
           break;
        }
        unsigned long field_count = mysql_num_fields(p_meta_res);
        if (field_count == 0)
        {
            last_error_msg_ = "mysql field_count is 0";
            last_error_ = MDbError::Unknown;
            break;
        }
        out_params_.resize(field_count);
        out_datas_.resize(field_count);
        out_lengths_.resize(field_count);
        out_is_nulls_.resize(field_count);
        for (size_t i = 0; i < field_count; ++i)
        {
            MBlob &out_data = out_datas_[i];
            out_data.Resize(p_field_list[i].max_length);
            memset(&(out_params_[i]), 0, sizeof(out_params_[i]));
            out_params_[i].buffer_type = p_field_list[i].type;
            out_params_[i].buffer = out_data.GetData();
            out_params_[i].buffer_length = out_data.GetSize();
            out_params_[i].length = &(out_lengths_[i]);
            out_params_[i].is_null = &(out_is_nulls_[i]);
            out_params_[i].is_unsigned = (p_field_list[i].flags & UNSIGNED_FLAG);
            out_params_[i].error = nullptr;
        }
        if (mysql_stmt_bind_result(p_stmt_, &out_params_[0]) != 0)
        {
            last_error_msg_ = MConcat("bind result failed errorno:", mysql_stmt_errno(p_stmt_), " error:", mysql_stmt_error(p_stmt_));
            last_error_ = MDbError::Unknown;
            break;
        }
    } while (0);
    mysql_free_result(p_meta_res);
    return last_error_;
}
