#ifndef _M_REDIS_CONNECTION_H_
#define _M_REDIS_CONNECTION_H_

#include <mzx/db/m_idb_connection.h>
#include <mzx/3rd/redis/hiredis.h>
#include <string>

class MRedisConnection
    :public MIDbConnection
{
public:
    MRedisConnection();
    virtual ~MRedisConnection();
public:
    redisContext* GetConnection();
private:
    virtual DbConnParamStyleType DoGetParamStyleType() override;
    virtual DbConnThreadSafetyType DoGetThreadSafetyType() override;
    virtual bool DoOpen(const std::string &conn_string) override;
    virtual void DoClose() override;
    virtual bool DoSelectDb(const std::string &db) override;
    virtual bool DoStartTran() override;
    virtual bool DoCommitTran() override;
    virtual bool DoRollBackTran() override;
    virtual MIDbCommand* DoCreateCommand() override;
private:
    redisContext *p_redis_;
};

#endif
