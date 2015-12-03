#ifndef _M_MYSQL_COMMAND_H_
#define _M_MYSQL_COMMAND_H_

#include <db/m_idb_command.h>
#include <util/m_convert.h>
#include <mysql.h>
#include <string>
#include <vector>
#include <cstring>
#include <util/m_type_define.h>

class MMysqlConnection;

class MMysqlCommand
    :public MIDbCommand
{
public:
    explicit MMysqlCommand(MMysqlConnection &conn);
    virtual ~MMysqlCommand();
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
    MDbError AddParamNumeric(enum_field_types buffer_type, my_bool is_unsigned, const T *p_param)
    {
        MYSQL_BIND bind;
        memset(&bind, 0, sizeof(bind));
        bind.buffer_type = buffer_type;
        bind.buffer = const_cast<T*>(p_param);
        bind.buffer_length = sizeof(*p_param);
        bind.length = nullptr;
        bind.is_null = nullptr;
        bind.is_unsigned = is_unsigned;
        in_params_.push_back(bind);
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        return last_error_;
    }
    template<typename T>
    MDbError GetParamNumeric(T *p_param)
    {
        if (cur_col_ >= out_params_.size())
        {
            last_error_msg_ = "record have no data";
            last_error_ = MDbError::NoData;
            return last_error_;
        }
        const MYSQL_BIND &bind = out_params_[cur_col_++];
        if (*(bind.is_null))
        {
            last_error_msg_ = "can't convert null to numeric";
            last_error_ = MDbError::ParamCannotConvert;
            return last_error_;
        }
        last_error_msg_ = "";
        last_error_ = MDbError::No;
        switch (bind.buffer_type)
        {
        case MYSQL_TYPE_TINY:
            if (bind.is_unsigned)
            {
                *p_param = static_cast<T>(*static_cast<uint8_t*>(bind.buffer));
            }
            else
            {
                *p_param = static_cast<T>(*static_cast<int8_t*>(bind.buffer));
            }
            break;
        case MYSQL_TYPE_SHORT:
            if (bind.is_unsigned)
            {
                *p_param = static_cast<T>(*static_cast<uint16_t*>(bind.buffer));
            }
            else
            {
                *p_param = static_cast<T>(*static_cast<int16_t*>(bind.buffer));
            }
            break;
        case MYSQL_TYPE_LONG:
            if (bind.is_unsigned)
            {
                *p_param = static_cast<T>(*static_cast<uint32_t*>(bind.buffer));
            }
            else
            {
                *p_param = static_cast<T>(*static_cast<int32_t*>(bind.buffer));
            }
            break;
        case MYSQL_TYPE_LONGLONG:
            if (bind.is_unsigned)
            {
                *p_param = static_cast<T>(*static_cast<uint64_t*>(bind.buffer));
            }
            else
            {
                *p_param = static_cast<T>(*static_cast<int64_t*>(bind.buffer));
            }
            break;
        case MYSQL_TYPE_FLOAT:
            *p_param = static_cast<T>(*static_cast<float*>(bind.buffer));
            break;
        case MYSQL_TYPE_DOUBLE:
            *p_param = static_cast<T>(*static_cast<double*>(bind.buffer));
            break;
        default:
            last_error_msg_ = MConcat("can't convert type:", bind.buffer_type, " to numeric");
            last_error_ = MDbError::ParamCannotConvert;
            break;
        }
        return last_error_;
    }
    MDbError BindResult();
private:
    MMysqlConnection &conn_;
    MYSQL_STMT *p_stmt_;
    std::vector<MYSQL_BIND> in_params_;
    std::vector<MYSQL_BIND> out_params_;
    std::vector<MBlob> out_datas_;
    std::vector<unsigned long> out_lengths_;
    std::vector<my_bool> out_is_nulls_;
    unsigned int cur_col_;
};

#endif
