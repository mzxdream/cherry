#ifndef _M_MYSQL_COMMAND_H_
#define _M_MYSQL_COMMAND_H_

#include <mzx/util/m_log.h>
#include <mzx/db/m_idb_command.h>
#include <mzx/util/m_convert.h>
#include <mzx/3rd/mysql/mysql.h>
#include <string>
#include <vector>
#include <cstring>

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
private:
    template<typename T>
    bool AddBaseTypeParam(unsigned int buffer_type, int is_unsigned, const T &param)
    {
        datas_.push_back(std::string(""));
        std::string &str = datas_.back();
        str.resize(sizeof(param));
        memcpy(const_cast<char*>(str.c_str()), &param, sizeof(param));
        MYSQL_BIND bind;
        memset(&bind, 0, sizeof(bind));
        bind.buffer_type = buffer_type;
        bind.buffer = const_cast<char*>(str.c_str());
        bind.buffer_length = str.size();
        bind.length = nullptr;
        bind.is_null = nullptr;
        bind.is_unsigned = is_unsigned;
        params_.push_back(bind);
        return true;
    }
    template<typename T>
    bool GetBaseTypeParam(T &param)
    {
    }
    bool CheckResult();
private:
    MMysqlConnection &conn_;
    MYSQL_STMT *p_stmt_;
    std::vector<MYSQL_BIND> params_;
    std::vector<std::string> datas_;
    std::vector<MYSQL_BIND> out_params_;
    std::vector<std::string> out_datas_;
    std::vector<unsigned long> out_lengths_;
    std::vector<my_bool> out_is_nulls_;
    unsigned int cur_row_;
    unsigned int cur_col_;
};

#endif
