#ifndef _M_IDB_CONNECTION_H_
#define _M_IDB_CONNECTION_H_

#include <string>
#include <db/m_db_common.h>

enum class MDbConnThreadSafetyType
{
    None = 0,
    Model = 1,
    Connect = 2,
    Command = 3,
};

enum class MDbConnParamStyleType
{
    CFormat = 0,
    QuestionMark = 1,
};

class MIDbCommand;

class MIDbConnection
{
public:
    MIDbConnection() = default;
    virtual ~MIDbConnection() = default;
    MIDbConnection(const MIDbConnection &) = delete;
    MIDbConnection& operator=(const MIDbConnection &) = delete;
public:
    MDbConnParamStyleType GetParamStyleType();
    MDbConnThreadSafetyType GetThreadSafetyType();
    MDbError Open(const std::string &conn_string);
    MDbError CheckConnect();
    MDbError CheckAndReConnect();
    void Close();
    MDbError SelectDb(const std::string &db);
    MIDbCommand* CreateCommand();
    MDbError GetLastError();
    std::string GetLastErrorMsg();
private:
    virtual MDbConnParamStyleType DoGetParamStyleType() = 0;
    virtual MDbConnThreadSafetyType DoGetThreadSafetyType() = 0;
    virtual MDbError DoOpen(const std::string &conn_string) = 0;
    virtual MDbError DoCheckConnect() = 0;
    virtual MDbError DoCheckAndReconnect() = 0;
    virtual void DoClose() = 0;
    virtual MDbError DoSelectDb(const std::string &db) = 0;
    virtual MIDbCommand* DoCreateCommand() = 0;
    virtual MDbError DoGetLastError() = 0;
    virtual std::string DoGetLastErrorMsg() = 0;
};

#endif
