#include "m_redis_connection.h"
#include <iostream>

MRedisConnection::MRedisConnection()
    :p_redis_(nullptr)
{

}

MRedisConnection::~MRedisConnection()
{
    this->MIDbConnection::Close();
}

DbConnParamStyleType MRedisConnection::DoGetParamStyleType()
{
    return DbConnParamStyleType.CFormat;
}

DbConnThreadSafetyType MRedisConnection::DoGetThreadSafetyType()
{
    return DbConnThreadSafetyType.Command;
}

bool MRedisConnection::DoOpen(const std::string &conn_string)
{
    if (p_redis_)
    {
        std::cerr << "redis is not null" << std::endl;
        return false;
    }
    p_redis_ = redisConnectWithTimeout();
    if (!p_redis_)
    {
        return false;
    }
    if (p_redis_->err)
    {
        return false;
    }
    return true;
}

void MRedisConnection::DoClose()
{
}

bool MRedisConnection::DoSelectDb(const std::string &db)
{
    return true;
}

bool MRedisConnection::DoStartTran()
{
    return true;
}

bool MRedisConnection::DoCommitTran()
{
    return true;
}

bool MRedisConnection::DoRollBackTran()
{
    return true;
}

MIDbCommand* MRedisConnection::DoCreateCommand()
{
    return nullptr;
}
