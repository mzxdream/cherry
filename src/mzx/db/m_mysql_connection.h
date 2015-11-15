#ifndef _M_MYSQL_CONNECTION_H_
#define _M_MYSQL_CONNECTION_H_

#include <mzx/db/m_idb_connection.h>
#include <mzx/3rd/mysql/mysql.h>
#include <string>

class MMysqlConnection
    :public MIDbConnection
{
public:
    MMysqlConnection();
    virtual ~MMysqlConnection();
public:
    MYSQL* GetConnection();
private:
    virtual DbConnParamStyleType DoGetParamStyleType() override;
    virtual DbConnThreadSafetyType DoGetThreadSafetyType() override;
    virtual bool DoOpen(const std::string &conn_string) override;
    virtual bool DoCheckConnect() override;
    virtual bool DoCheckAndReconnect() override;//mysql connect string need set reconnect;
    virtual void DoClose() override;
    virtual bool DoSelectDb(const std::string &db) override;
    virtual MIDbCommand* DoCreateCommand() override;
private:
    MYSQL *p_mysql_;
};

#endif
