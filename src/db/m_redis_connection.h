#ifndef _M_REDIS_CONNECTION_H_
#define _M_REDIS_CONNECTION_H_

#include <db/m_idb_connection.h>
#include <hiredis.h>
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
    MDbError AuthPwd(const std::string &pwd);
private:
    virtual MDbConnParamStyleType DoGetParamStyleType() override;
    virtual MDbConnThreadSafetyType DoGetThreadSafetyType() override;
    virtual MDbError DoOpen(const std::string &conn_string) override;
    virtual MDbError DoCheckConnect() override;
    virtual MDbError DoCheckAndReconnect() override;
    virtual void DoClose() override;
    virtual MDbError DoSelectDb(const std::string &db) override;
    virtual MIDbCommand* DoCreateCommand() override;
    virtual MDbError DoGetLastError() override;
    virtual std::string DoGetLastErrorMsg() override;
private:
    redisContext *p_redis_;
    std::string conn_string_;
    MDbError last_error_;
    std::string last_error_msg_;
};

#endif
