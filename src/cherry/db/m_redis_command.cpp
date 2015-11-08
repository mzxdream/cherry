#include "m_redis_command.h"
#include "m_redis_connection.h"

MRedisCommand::MRedisCommand(MRedisConnection &conn)
    :conn_(conn)
{
}

MRedisCommand::~MRedisCommand()
{
}

bool MRedisCommand::DoPrepair(const std::string &command)
{
    args_.push_back(command);
    return true;
}

bool MRedisCommand::DoBeforeAddParam()
{
    args_.resize(1);
    return true;
}

int MRedisCommand::DoExecuteNonQuery()
{
    return 0;
}

bool MRedisCommand::DoExecuteReader()
{
    return true;
}

bool MRedisCommand::DoAddParam(const int8_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const uint8_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const int16_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const uint16_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const int32_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const uint32_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const int64_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const uint64_t &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const float &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const double &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const std::string &param)
{
    return true;
}

bool MRedisCommand::DoAddParam(const MBlob &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(int8_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(uint8_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(int16_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(uint16_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(int32_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(uint32_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(int64_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(uint64_t &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(float &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(double &param)
{
    return true;
}

bool MRedisCommand::DoGetParam(std::string &param)
{
    return true;
}
