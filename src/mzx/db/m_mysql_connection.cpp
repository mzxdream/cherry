#include <mzx/db/m_mysql_connection.h>
#include <mzx/db/m_mysql_command.h>
#include <mzx/util/m_log.h>
#include <mzx/util/m_convert.h>
#include <mzx/util/m_string.h>

static const std::string sc_mysql_sep = ";";
static const std::string sc_mysql_op = "=";
static const std::string sc_mysql_trim = " ";
static const std::string sc_mysql_ip = "ip";
static const std::string sc_mysql_port = "port";
static const std::string sc_mysql_timeout = "timeout";
static const std::string sc_mysql_user = "user";
static const std::string sc_mysql_pwd = "pwd";
static const std::string sc_mysql_db = "db";
static const std::string sc_mysql_reconnect = "reconnect";
static const std::string sc_mysql_charset = "charset";
static const std::string sc_mysql_client_flag = "clientflag";

MMysqlConnection::MMysqlConnection()
    :p_mysql_(nullptr)
{
}

MMysqlConnection::~MMysqlConnection()
{
    if (p_mysql_)
    {
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
    }
}

MYSQL* MMysqlConnection::GetConnection()
{
    return p_mysql_;
}

DbConnParamStyleType MMysqlConnection::DoGetParamStyleType()
{
    return DbConnParamStyleType::QuestionMark;
}

DbConnThreadSafetyType MMysqlConnection::DoGetThreadSafetyType()
{
    return DbConnThreadSafetyType::Connect;
}

bool MMysqlConnection::DoOpen(const std::string &conn_string)
{
    if (p_mysql_)
    {
        MLOG(Error) << "mysql is not null";
        return false;
    }
    if ((p_mysql_ = mysql_init(nullptr)) == nullptr)
    {
        MLOG(Error) << "init mysql failed";
        return false;
    }
    unsigned int timeout = 0;
    if (MString::GetParamValue(conn_string, sc_mysql_timeout, timeout, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        if (mysql_options(p_mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &timeout) != 0)
        {
            MLOG(Error) << "set timeout failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
            mysql_close(p_mysql_);
            p_mysql_ = nullptr;
            return false;
        }
    }

    char reconnect = 0;
    if (MString::GetParamValue(conn_string, sc_mysql_reconnect, reconnect, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        if (mysql_options(p_mysql_, MYSQL_OPT_RECONNECT, &reconnect) != 0)
        {
            MLOG(Error) << "set reconnect failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
            mysql_close(p_mysql_);
            p_mysql_ = nullptr;
            return false;
        }
    }

    std::string ip;
    const char *p_ip = nullptr;
    if (MString::GetParamValue(conn_string, sc_mysql_ip, ip, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        p_ip = ip.c_str();
    }
    unsigned int port = 0;
    MString::GetParamValue(conn_string, sc_mysql_port, port, sc_mysql_op, sc_mysql_sep, sc_mysql_trim);
    std::string user;
    const char *p_user = nullptr;
    if (MString::GetParamValue(conn_string, sc_mysql_user, user, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        p_user = user.c_str();
    }
    std::string pwd;
    const char *p_pwd = nullptr;
    if (MString::GetParamValue(conn_string, sc_mysql_pwd, pwd, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        p_pwd = pwd.c_str();
    }
    std::string db;
    const char *p_db = nullptr;
    if (MString::GetParamValue(conn_string, sc_mysql_db, db, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        p_db = db.c_str();
    }
    unsigned int client_flag = 0;
    MString::GetParamValue(conn_string, sc_mysql_client_flag, client_flag, sc_mysql_op, sc_mysql_sep, sc_mysql_trim);

    if (mysql_real_connect(p_mysql_, p_ip, p_user, p_pwd, p_db, port, nullptr, client_flag) != p_mysql_)
    {
        MLOG(Error) << "connect failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
        return false;
    }

    std::string charset;
    if (MString::GetParamValue(conn_string, sc_mysql_charset, charset, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
    {
        if (mysql_set_character_set(p_mysql_, charset.c_str()) != 0)
        {
            MLOG(Error) << "set charset failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
            return false;
        }
    }
    return true;
}

bool MMysqlConnection::DoCheckConnect()
{
    if (mysql_query(p_mysql_, "select 1+1") != 0)
    {
        MLOG(Error) << "select 1+1 failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        return false;
    }
    MYSQL_RES *p_res = mysql_store_result(p_mysql_);
    if (!p_res)
    {
        MLOG(Error) << "get res failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        return false;
    }
    MYSQL_ROW row = mysql_fetch_row(p_res);
    if (!row)
    {
        MLOG(Error) << "get row failed errorno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        mysql_free_result(p_res);
        return false;
    }
    if (mysql_num_fields(p_res) != 1)
    {
        MLOG(Error) << "fields is not 1";
        mysql_free_result(p_res);
        return false;
    }
    unsigned long *p_lengths = mysql_fetch_lengths(p_res);
    if (!p_lengths)
    {
        MLOG(Error) << "fields is not 1";
        mysql_free_result(p_res);
        return false;
    }
    int result = 0;
    if (!MConvert::StrToBaseType(std::string(row[0], p_lengths[0]), result)
        || result != 2)
    {
        MLOG(Error) << "result is wrong";
        mysql_free_result(p_res);
        return false;
    }
    mysql_free_result(p_res);
    return true;
}

bool MMysqlConnection::DoCheckAndReconnect()
{
    if (mysql_ping(p_mysql_) != 0)
    {
        MLOG(Error) << "reconnect failed errno:" << mysql_errno(p_mysql_) << " error:" << mysql_error(p_mysql_);
        return false;
    }
    return true;
}

void MMysqlConnection::DoClose()
{
    if (p_mysql_)
    {
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
    }
}

bool MMysqlConnection::DoSelectDb(const std::string &db)
{
    int ret = mysql_select_db(p_mysql_, db.c_str());
    if (ret != 0)
    {
        MLOG(Error) << "select db failed error:" << ret;
        return false;
    }
    return true;
}

MIDbCommand* MMysqlConnection::DoCreateCommand()
{
    return new MMysqlCommand(*this);
}
