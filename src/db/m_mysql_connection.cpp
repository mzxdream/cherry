#include <db/m_mysql_connection.h>
#include <db/m_mysql_command.h>
#include <util/m_convert.h>
#include <util/m_string.h>

static const std::string sc_mysql_sep = ";";
static const std::string sc_mysql_op = "=";
static const std::string sc_mysql_trim = " ";
static const std::string sc_mysql_ip = "ip";
static const std::string sc_mysql_port = "port";
static const std::string sc_mysql_timeout = "timeout";
static const std::string sc_mysql_user = "user";
static const std::string sc_mysql_pwd = "pwd";
static const std::string sc_mysql_db = "db";
static const std::string sc_mysql_charset = "charset";
static const std::string sc_mysql_client_flag = "clientflag";

MMysqlConnection::MMysqlConnection()
    :p_mysql_(nullptr)
    ,last_error_(MDbError::No)
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

MDbConnParamStyleType MMysqlConnection::DoGetParamStyleType()
{
    return MDbConnParamStyleType::QuestionMark;
}

MDbConnThreadSafetyType MMysqlConnection::DoGetThreadSafetyType()
{
    return MDbConnThreadSafetyType::Connect;
}

MDbError MMysqlConnection::DoOpen(const std::string &conn_string)
{
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    if (p_mysql_)
    {
        last_error_msg_ = "mysql is opened";
        last_error_ = MDbError::ConnectOpened;
        return last_error_;
    }
    if ((p_mysql_ = mysql_init(nullptr)) == nullptr)
    {
        last_error_msg_ = "memory full";
        last_error_ = MDbError::OutOfMemory;
        return last_error_;
    }
    do
    {
        unsigned int timeout = 0;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_timeout, timeout, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            if (mysql_options(p_mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &timeout) != 0)
            {
                last_error_msg_ = MConcat("set timeout failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
                last_error_ = MDbError::SetParamFailed;
                break;
            }
        }

        char reconnect = 1;
        if (mysql_options(p_mysql_, MYSQL_OPT_RECONNECT, &reconnect) != 0)
        {
            last_error_msg_ = MConcat("set reconnect failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
            last_error_ = MDbError::SetParamFailed;
            break;
        }

        std::string ip;
        const char *p_ip = nullptr;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_ip, ip, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            p_ip = ip.c_str();
        }
        unsigned int port = 0;
        MDbConnString::GetParamValue(conn_string, sc_mysql_port, port, sc_mysql_op, sc_mysql_sep, sc_mysql_trim);
        std::string user;
        const char *p_user = nullptr;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_user, user, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            p_user = user.c_str();
        }
        std::string pwd;
        const char *p_pwd = nullptr;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_pwd, pwd, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            p_pwd = pwd.c_str();
        }
        std::string db;
        const char *p_db = nullptr;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_db, db, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            p_db = db.c_str();
        }
        unsigned int client_flag = 0;
        MDbConnString::GetParamValue(conn_string, sc_mysql_client_flag, client_flag, sc_mysql_op, sc_mysql_sep, sc_mysql_trim);

        if (mysql_real_connect(p_mysql_, p_ip, p_user, p_pwd, p_db, port, nullptr, client_flag) != p_mysql_)
        {
            last_error_msg_ = MConcat("connect failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
            last_error_ = MDbError::ConnectFailed;
            break;
        }

        std::string charset;
        if (MDbConnString::GetParamValue(conn_string, sc_mysql_charset, charset, sc_mysql_op, sc_mysql_sep, sc_mysql_trim))
        {
            if (mysql_set_character_set(p_mysql_, charset.c_str()) != 0)
            {
                last_error_msg_ = MConcat("set charset failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
                last_error_ = MDbError::SetParamFailed;
                break;
           }
        }
    } while (0);
    if (last_error_ != MDbError::No)
    {
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
    }
    return last_error_;
}

MDbError MMysqlConnection::DoCheckConnect()
{
    if (mysql_query(p_mysql_, "select 1+1") != 0)
    {
        last_error_msg_ = MConcat("select 1+1 failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
        last_error_ = MDbError::QueryFailed;
        return last_error_;
    }
    MYSQL_RES *p_res = mysql_store_result(p_mysql_);
    if (!p_res)
    {
        last_error_msg_ = MConcat("get res failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
        last_error_ = MDbError::GetResFailed;
        return last_error_;
    }
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    do
    {
        MYSQL_ROW row = mysql_fetch_row(p_res);
        if (!row)
        {
            last_error_msg_ = MConcat("get row failed errorno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
            last_error_ = MDbError::GetRowFailed;
            break;
        }
        if (mysql_num_fields(p_res) != 1)
        {
            last_error_msg_ = "fields is not 1";
            last_error_ = MDbError::Unknown;
            break;
        }
        unsigned long *p_lengths = mysql_fetch_lengths(p_res);
        if (!p_lengths)
        {
            last_error_msg_ = "lengths is null";
            last_error_ = MDbError::Unknown;
            break;
        }
        int result = 0;
        if (!MConvertTo(std::string(row[0], p_lengths[0]), result)
                || result != 2)
        {
            last_error_msg_ = "result is wrong";
            last_error_ = MDbError::Unknown;
            break;
        }
    } while(0);
    if (last_error_ != MDbError::No)
    {
       mysql_free_result(p_res);
       p_res = nullptr;
    }
    return last_error_;
}

MDbError MMysqlConnection::DoCheckAndReconnect()
{
    last_error_msg_ = "";
    last_error_ = MDbError::No;
    if (mysql_ping(p_mysql_) != 0)
    {
        last_error_msg_ = MConcat("reconnect failed errno:", mysql_errno(p_mysql_), " error:", mysql_error(p_mysql_));
        last_error_ = MDbError::ConnectFailed;
    }
    return last_error_;
}

void MMysqlConnection::DoClose()
{
    if (p_mysql_)
    {
        mysql_close(p_mysql_);
        p_mysql_ = nullptr;
    }
}

MDbError MMysqlConnection::DoSelectDb(const std::string &db)
{
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    int ret = mysql_select_db(p_mysql_, db.c_str());
    if (ret != 0)
    {
        last_error_msg_ = MConcat("select db failed error:", ret);
        last_error_ = MDbError::Unknown;
    }
    return last_error_;
}

MIDbCommand* MMysqlConnection::DoCreateCommand()
{
    return new MMysqlCommand(*this);
}
