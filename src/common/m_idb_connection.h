#ifndef _M_IDB_CONNECTION_H_
#define _M_IDB_CONNECTION_H_

#include <string>

enum class DbConnThreadSafetyType
{
    None = 0,
    Model = 1,
    Connect = 2,
    Command = 3,
};

enum class DbConnParamStyleType
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
    virtual DbConnParamStyleType GetParamStyleType() = 0;
    virtual DbConnThreadSafetyType GetThreadSafetyType() = 0;
    virtual bool Open(const std::string &conn_string) = 0;
    virtual void Close() = 0;
    virtual bool SelectDb(const std::string &db)
    {
        return false;
    }
    virtual bool StartTran()
    {
        return false;
    }
    virtual bool CommitTran()
    {
        return false;
    }
    virtual bool RollBackTran()
    {
        return false;
    }
    virtual MIDbCommand* CreateCommand() = 0;
};

#endif
