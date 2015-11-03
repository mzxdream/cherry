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
    DbConnParamStyleType GetParamStyleType()
    {
        return DoGetParamStyleType();
    }
    DbConnThreadSafetyType GetThreadSafetyType()
    {
        return DoGetThreadSafetyType();
    }
    bool Open(const std::string &conn_string)
    {
        return DoOpen(conn_string);
    }
    void Close()
    {
        DoClose();
    }
    bool SelectDb(const std::string &db)
    {
        return DoSelectDb(db);
    }
    bool StartTran()
    {
        return DoStartTran();
    }
    bool CommitTran()
    {
        return DoCommitTran();
    }
    virtual bool RollBackTran()
    {
        return DoRollBackTran();
    }
    MIDbCommand* CreateCommand()
    {
        return DoCreateCommand();
    }
private:
    virtual DbConnParamStyleType DoGetParamStyleType() = 0;
    virtual DbConnThreadSafetyType DoGetThreadSafetyType() = 0;
    virtual bool DoOpen(const std::string &conn_string) = 0;
    virtual void DoClose() = 0;
    virtual bool DoSelectDb(const std::string &db)
    {
        return false;
    }
    virtual bool DoStartTran()
    {
        return false;
    }
    virtual bool DoCommitTran()
    {
        return false;
    }
    virtual bool DoRollBackTran()
    {
        return false;
    }
    virtual MIDbCommand* DoCreateCommand() = 0;
};

#endif
