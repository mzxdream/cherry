#include "m_redis_connection.h"
#include <cherry/util/m_log.h>
#include <cherry/util/m_convert.h>
#include <iostream>
#include <sys/time.h>

static const sc_redis_separator = ";";
static const sc_redis_ip = "ip=";
static const sc_redis_port = "port=";
static const sc_redis_timeout = "timeout=";
static const sc_redis_pwd = "pwd=";
static const sc_redis_db = "db=";

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

DbConnParamStyleType MRedisConnection::DoGetParamStyleType()
{
    return DbConnParamStyleType::CFormat;
}

DbConnThreadSafetyType MRedisConnection::DoGetThreadSafetyType()
{
    return DbConnThreadSafetyType::Command;
}

bool MRedisConnection::DoOpen(const std::string &conn_string)
{
    if (p_redis_)
    {
        MLOG(Error) << "redis is not null";
        return false;
    }

    size_t ip_pos_start = conn_string.find_last_of(sc_redis_ip);
    if (ip_pos_start == std::npos)
    {
        MLOG(Error) << "can't get ip info";
        return false;
    }
    ip_pos_start += sc_redis_ip.size();
    size_t ip_pos_end = conn_string.find_first_of(sc_redis_separator, ip_pos_start);
    if (ip_pos_end == std::npos)
    {
        MLOG(Error) << "can't get ip info end";
        return false;
    }
    std::string ip = conn_string.substr(ip_pos_start, ip_pos_end-ip_pos_start);

    size_t port_pos_start = conn_string.find_last_of(sc_redis_port);
    if (port_pos_start == std::npos)
    {
        MLOG(Error) << "can't get port info";
        return false;
    }
    port_pos_start += sc_redis_port.size();
    size_t port_pos_end = conn_string.find_first_of(sc_redis_separator, port_pos_start);
    if (port_pos_end == std::npos)
    {
        MLOG(Error) << "can't get port info end";
        return false;
    }
    int port = 0;
    if (!MConvert::StrToBaseType(conn_string.substr(port_pos_start, port_pos_end-port_pos_start), port))
    {
        MLOG(Error) << "can't convert port";
        return false;
    }

    size_t timeout_pos_start = conn_string.find_last_of(sc_redis_timeout);
    if (timeout_pos_start == std::npos)
    {
        p_redis_ = redisConnect(ip.c_str(), port);
    }
    else
    {
        timeout_pos_start += sc_redis_timeout.size();
        size_t timeout_pos_end = conn_string.find_first_of(sc_redis_separator, timeout_pos_start);
        if (timeout_pos_end == std::npos)
        {
            MLOG(Error) << "can't get timeout end";
            return false;
        }
        time_t timeout = 0;
        if (!MConvert::StrToBaseType(conn_string.substr(timeout_pos_start, timeout_pos_end-timeout_pos_start), timeout))
        {
            MLOG(Error) << "can't convert timeout";
            return false;
        }
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

    size_t pwd_pos_start = conn_string.find_last_of(sc_redis_pwd);
    if (pwd_pos_start != std::npos)
    {
        pwd_pos_start += sc_redis_pwd.size();
        size_t pwd_pos_end = conn_string.find_first_of(sc_redis_separator, pwd_pos_start);
        std::string pwd = conn_string.substr(pwd_pos_start, pwd_pos_end-pwd_pos_start);
        redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "AUTH %s", pwd.c_str()));
        if (!p_reply)
        {
            MLOG(Error) << "auth pwd failed reply is null";
            redisFree(p_redis_);
            p_redis_ = nullptr;
            return false;
        }
        if (p_reply->type == REDIS_REPLY_ERROR)
        {
            MLOG(Error) << "auth pwd failed error:" << p_reply->str;
            redisFree(p_redis_);
            p_redis_ = nullptr;
            freeReplyObject(p_reply);
            p_reply = nullptr;
            return false;
        }
        freeReplyObject(p_reply);
        p_reply = nullptr;
    }

    size_t db_pos_start = conn_string.find_last_of(sc_redis_db);
    if (db_pos_start != std::npos)
    {
        db_pos_start += sc_redis_db.size();
        size_t db_pos_end = conn_string.find_first_of(sc_redis_separator, db_pos_start);
        std::string db = conn_string.substr(db_pos_start, db_pos_end-db_pos_start);
        redisReply *p_reply = static_cast<redisReply*>(redisCommand(p_redis_, "SELECT %s", db.c_str());
        if (!p_reply)
        {
            MLOG(Error) << "select db failed reply is null";
            redisFree(p_redis_);
            p_redis_ = nullptr;
            return false;
        }
        if (p_reply->type == REDIS_REPLY_ERROR)
        {
            MLOG(Error) << "select db failed error:" << p_reply->str;
            redisFree(p_redis_);
            p_redis_ = nullptr;
            freeReplyObject(p_reply);
            p_reply = nullptr;
            return false;
        }
        freeReplyObject(p_reply);
        p_reply = nullptr;
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
    if (p_reply->type == REDIS_REPLY_ERROR)
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

bool MRedisConnection::DoStartTran()
{
    return false;
}

bool MRedisConnection::DoCommitTran()
{
    return false;
}

bool MRedisConnection::DoRollBackTran()
{
    return false;
}

MIDbCommand* MRedisConnection::DoCreateCommand()
{
    return nullptr;
}
