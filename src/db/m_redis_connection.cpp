#include <db/m_redis_connection.h>
#include <db/m_redis_command.h>
#include <db/m_db_conn_string.h>
#include <util/m_log.h>
#include <util/m_convert.h>
#include <util/m_string.h>
#include <iostream>
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

bool MRedisConnection::AuthPwd(const std::string &pwd)
{
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "AUTH %s", pwd.c_str()));
    if (!p_reply)
    {
        MLOG(Error) << "auth pwd failed reply is null";
        return false;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "OK") != 0))
    {
        MLOG(Error) << "auth pwd failed error:" << p_reply->str;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return false;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    return true;
}

DbConnParamStyleType MRedisConnection::DoGetParamStyleType()
{
    return DbConnParamStyleType::CFormat;
}

DbConnThreadSafetyType MRedisConnection::DoGetThreadSafetyType()
{
    return DbConnThreadSafetyType::Connect;
}

bool MRedisConnection::DoOpen(const std::string &conn_string)
{
    if (p_redis_)
    {
        MLOG(Error) << "redis is not null";
        return false;
    }

    std::string ip;
    if (!MDbConnString::GetParamValue(conn_string, sc_redis_ip, ip, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        MLOG(Error) << "can't get ip info";
        return false;
    }

    int port = 0;
    if (!MDbConnString::GetParamValue(conn_string, sc_redis_port, port, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        MLOG(Error) << "can't get port info";
        return false;
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
        MLOG(Error) << "redis connect is null";
        return false;
    }
    if (p_redis_->err != REDIS_OK)
    {
        MLOG(Error) << "connect error:" << p_redis_->errstr;
        redisFree(p_redis_);
        p_redis_ = nullptr;
        return false;
    }

    std::string pwd;
    if (MDbConnString::GetParamValue(conn_string, sc_redis_pwd, pwd, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        if (!AuthPwd(pwd))
        {
            MLOG(Error) << "auth pwd:" << pwd << " failed";
            redisFree(p_redis_);
            p_redis_ = nullptr;
            return false;
        }
    }

    std::string db;
    if (MDbConnString::GetParamValue(conn_string, sc_redis_db, db, sc_redis_op, sc_redis_sep, sc_redis_trim))
    {
        if (!SelectDb(db))
        {
            MLOG(Error) << "select db:" << db << " failed";
            redisFree(p_redis_);
            p_redis_ = nullptr;
            return false;
        }
    }
    conn_string_ = conn_string;
    return true;
}

bool MRedisConnection::DoCheckConnect()
{
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "PING"));
    if (!p_reply)
    {
        MLOG(Error) << "ping failed reply is null";
        return false;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "PONG") != 0))
    {
        MLOG(Error) << "ping failed error:" << p_reply->str;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return false;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    return true;
}

bool MRedisConnection::DoCheckAndReconnect()
{
    if (!CheckConnect())
    {
        return Open(conn_string_);
    }
    return true;
}

void MRedisConnection::DoClose()
{
    if (p_redis_)
    {
        redisFree(p_redis_);
        p_redis_ = nullptr;
    }
}

bool MRedisConnection::DoSelectDb(const std::string &db)
{
    redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "SELECT %s", db.c_str()));
    if (!p_reply)
    {
        MLOG(Error) << "select db failed reply is null";
        return false;
    }
    if (p_reply->type == REDIS_REPLY_ERROR
        || (p_reply->type == REDIS_REPLY_STATUS && MString::CompareNoCase(p_reply->str, "OK") != 0))
    {
        MLOG(Error) << "select db failed error:" << p_reply->str;
        freeReplyObject(p_reply);
        p_reply = nullptr;
        return false;
    }
    freeReplyObject(p_reply);
    p_reply = nullptr;
    return true;
}

MIDbCommand* MRedisConnection::DoCreateCommand()
{
    return new MRedisCommand(*this);
}
