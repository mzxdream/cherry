#ifndef _M_MYSQL_COMMAND_H_
#define _M_MYSQL_COMMAND_H_

#include <util/m_log.h>
#include <db/m_idb_command.h>
#include <util/m_convert.h>
#include <3rd/mysql/mysql.h>
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
    virtual bool DoGetParam(MBlob &param) override;
private:
    template<typename T>
    bool AddBaseTypeParam(enum_field_types buffer_type, my_bool is_unsigned, const T &param)
    {
        datas_.resize(datas_.size()+1);
        MBlob &data = datas_.back();
        data.Resize(sizeof(param));
        memcpy(data.GetData(), &param, data.GetSize());
        MYSQL_BIND bind;
        memset(&bind, 0, sizeof(bind));
        bind.buffer_type = buffer_type;
        bind.buffer = data.GetData();
        bind.buffer_length = data.GetSize();
        bind.length = nullptr;
        bind.is_null = nullptr;
        bind.is_unsigned = is_unsigned;
        params_.push_back(bind);
        return true;
    }
    template<typename T>
    bool GetBaseTypeParam(T &param)
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
            MLOG(Error) << "param is null";
            return false;
        }
        bool ret = false;
        switch (bind.buffer_type)
        {
        case MYSQL_TYPE_TINY:
            if (bind.is_unsigned)
            {
                param = static_cast<T>(*static_cast<uint8_t*>(bind.buffer));
            }
            else
            {
                param = static_cast<T>(*static_cast<int8_t*>(bind.buffer));
            }
            ret = true;
            break;
        case MYSQL_TYPE_SHORT:
            if (bind.is_unsigned)
            {
                param = static_cast<T>(*static_cast<uint16_t*>(bind.buffer));
            }
            else
            {
                param = static_cast<T>(*static_cast<int16_t*>(bind.buffer));
            }
            ret = true;
            break;
        case MYSQL_TYPE_LONG:
            if (bind.is_unsigned)
            {
                param = static_cast<T>(*static_cast<uint32_t*>(bind.buffer));
            }
            else
            {
                param = static_cast<T>(*static_cast<int32_t*>(bind.buffer));
            }
            ret = true;
            break;
        case MYSQL_TYPE_LONGLONG:
            if (bind.is_unsigned)
            {
                param = static_cast<T>(*static_cast<uint64_t*>(bind.buffer));
            }
            else
            {
                param = static_cast<T>(*static_cast<int64_t*>(bind.buffer));
            }
            ret = true;
            break;
        case MYSQL_TYPE_FLOAT:
            param = static_cast<T>(*static_cast<float*>(bind.buffer));
            ret = true;
            break;
        case MYSQL_TYPE_DOUBLE:
            param = static_cast<T>(*static_cast<double*>(bind.buffer));
            ret = true;
            break;
        default:
            MLOG(Error) << "can't convert type:" << bind.buffer_type << " to base type";
            ret = false;
            break;
        }
        return ret;
    }
    bool BindResult();
    bool FetchNextRow();
    bool FetchNextResult();
    bool CheckResult();
private:
    MMysqlConnection &conn_;
    MYSQL_STMT *p_stmt_;
    std::vector<MYSQL_BIND> params_;
    std::vector<MBlob> datas_;
    std::vector<MYSQL_BIND> out_params_;
    std::vector<MBlob> out_datas_;
    std::vector<unsigned long> out_lengths_;
    std::vector<my_bool> out_is_nulls_;
    unsigned int cur_row_;
    unsigned int cur_col_;
};

#endif
