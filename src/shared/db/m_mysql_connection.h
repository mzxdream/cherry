#ifndef _M_MYSQL_CONNECTION_H_
#define _M_MYSQL_CONNECTION_H_

#include <db/m_idb_connection.h>
#include <mysql.h>
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
    virtual MDbConnParamStyleType DoGetParamStyleType() override;
    virtual MDbConnThreadSafetyType DoGetThreadSafetyType() override;
    virtual MDbError DoOpen(const std::string &conn_string) override;
    virtual MDbError DoCheckConnect() override;
    virtual MDbError DoCheckAndReconnect() override;
    virtual void DoClose() override;
    virtual MDbError DoSelectDb(const std::string &db) override;
    virtual MIDbCommand* DoCreateCommand() override;
private:
    MYSQL *p_mysql_;
};

#endif
