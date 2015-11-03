#ifndef _M_REDIS_CONNECTION_H_
#define _M_REDIS_CONNECTION_H_

#include "common/m_idb_connection.h"
#include "common/m_noncopyable.h"
#include "3rd/redis/hiredis.h"

class MRedisConnection
    :public MIDbConnection
{
public:
    MRedisConnection();
    virtual ~MRedisConnection();
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
