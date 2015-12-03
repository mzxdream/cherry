#include <db/m_redis_connection.h>
#include <db/m_redis_command.h>
#include <util/m_convert.h>
#include <util/m_string.h>
#include <sys/time.h>
#include <string>

static const std::string sc_redis_sep = ";";
static const std::string sc_redis_op = "=";
static const std::string sc_redis_trim = " ";
static const std::string sc_redis_ip = "ip";
static const std::string sc_redis_port = "port";
static const std::string sc_redis_timeout = "timeout";
static const std::string sc_redis_pwd = "pwd";
static const std::string sc_redis_db = "db";

MRedisConnection::MRedisConnection()
    :p_redis_(nullptr)
{
}

MRedisConnection::~MRedisConnection()
{
    if (p_redis_)
    {
        redisFree(p_redis_);
        p_redis_ = nullptr;
    }
}

redisContext* MRedisConnection::GetConnection()
{
    return p_redis_;
}

MDbError MRedisConnection::AuthPwd(const std::string &pwd)
{
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "AUTH %s", pwd.c_str()));
    if (!p_reply)
    {
        last_error_msg_ = "auth pwd failed reply is null";
        last_error_ = MDbError::Unknown;
        return last_error_;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "OK") != 0))
    {
        last_error_msg_ = MConcat("auth pwd failed error:", p_reply->str);
        last_error_ = MDbError::Unknown;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return last_error_;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    return last_error_;
}

MDbConnParamStyleType MRedisConnection::DoGetParamStyleType()
{
    return MDbConnParamStyleType::CFormat;
}

MDbConnThreadSafetyType MRedisConnection::DoGetThreadSafetyType()
{
    return MDbConnThreadSafetyType::Connect;
}

MDbError MRedisConnection::DoOpen(const std::string &conn_string)
{
    if (p_redis_)
    {
        last_error_msg_ = "redis is opened";
        last_error_ = MDbError::ConnectOpened;
        return last_error_;
    }

    std::string ip;
    if (!MDbConnString::GetParamValue(conn_string, sc_redis_ip, ip, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        last_error_msg_ = "can't get ip info";
        last_error_ = MDbError::GetParamFailed;
        return last_error_;
    }

    int port = 0;
    if (!MDbConnString::GetParamValue(conn_string, sc_redis_port, port, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        last_error_msg_ = "can't get port info";
        last_error_ = MDbError::GetParamFailed;
        return last_error_;
    }

    time_t timeout = 0;
    if (!MDbConnString::GetParamValue(conn_string, sc_redis_timeout, timeout, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        p_redis_ = redisConnect(ip.c_str(), port);
    }
    else
    {
        struct timeval tm;
        tm.tv_sec = timeout;
        tm.tv_usec = 0;
        p_redis_ = redisConnectWithTimeout(ip.c_str(), port, tm);
    }

    if (!p_redis_)
    {
        last_error_msg_ = "redis connect is null";
        last_error_ = MDbError::ConnectFailed;
        return last_error_;
    }
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    do
    {
        if (p_redis_->err != REDIS_OK)
        {
            last_error_msg_ = MConcat("connect error:", p_redis_->errstr);
            last_error_ = MDbError::ConnectFailed;
            break;
        }

        std::string pwd;
        if (MDbConnString::GetParamValue(conn_string, sc_redis_pwd, pwd, sc_redis_op, sc_redis_sep, sc_redis_trim))
        {
            if (AuthPwd(pwd) != MDbError::No)
            {
                break;
            }
        }

        std::string db;
        if (MDbConnString::GetParamValue(conn_string, sc_redis_db, db, sc_redis_op, sc_redis_sep, sc_redis_trim))
        {
            if (SelectDb(db) != MDbError::No)
            {
                break;
            }
        }
        conn_string_ = conn_string;
    } while (0);
    if (last_error_ != MDbError::No)
    {
        redisFree(p_redis_);
        p_redis_ = nullptr;
    }
    return last_error_;
}

MDbError MRedisConnection::DoCheckConnect()
{
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "PING"));
    if (!p_reply)
    {
        last_error_msg_ = "ping failed reply is null";
        last_error_ = MDbError::QueryFailed;
        return last_error_;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "PONG") != 0))
    {
        last_error_msg_ = MConcat("ping failed error:", p_reply->str);
        last_error_ = MDbError::Disconnect;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return last_error_;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    return last_error_;
}

MDbError MRedisConnection::DoCheckAndReconnect()
{
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    if (CheckConnect() == MDbError::Disconnect)
    {
        Close();
        return Open(conn_string_);
    }
    return last_error_;
}

void MRedisConnection::DoClose()
{
    if (p_redis_)
    {
        redisFree(p_redis_);
        p_redis_ = nullptr;
    }
}

MDbError MRedisConnection::DoSelectDb(const std::string &db)
{
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "SELECT %s", db.c_str()));
    if (!p_reply)
    {
        last_error_msg_ = "select db failed reply is null";
        last_error_ = MDbError::QueryFailed;
        return last_error_;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "OK") != 0))
    {
        last_error_msg_ = MConcat("select db failed error:", p_reply->str);
        last_error_ = MDbError::Unknown;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return last_error_;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    last_error_ = MDbError::No;
    last_error_msg_ = "";
    return last_error_;
}

MIDbCommand* MRedisConnection::DoCreateCommand()
{
    return new MRedisCommand(*this);
}
