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
    MIDbConnection()
        :last_error_(MDbError::No)
    {
    }
    virtual ~MIDbConnection() = default;
    MIDbConnection(const MIDbConnection &) = delete;
    MIDbConnection& operator=(const MIDbConnection &) = delete;
public:
    MDbConnParamStyleType GetParamStyleType() { return DoGetParamStyleType(); }
    MDbConnThreadSafetyType GetThreadSafetyType() { return DoGetThreadSafetyType(); }
    MDbError Open(const std::string &conn_string) { return DoOpen(conn_string); }
    MDbError CheckConnect() { return DoCheckConnect(); }
    MDbError CheckAndReConnect() { return DoCheckAndReconnect(); }
    void Close() { return DoClose(); }
    MDbError SelectDb(const std::string &db) { return DoSelectDb(db); }
    MIDbCommand* CreateCommand() { return DoCreateCommand(); }
    MDbError GetLastError() const { return last_error_; }
    const std::string& GetLastErrorMsg() const { return last_error_msg_; }
private:
    virtual MDbConnParamStyleType DoGetParamStyleType() = 0;
    virtual MDbConnThreadSafetyType DoGetThreadSafetyType() = 0;
    virtual MDbError DoOpen(const std::string &conn_string) = 0;
    virtual MDbError DoCheckConnect() = 0;
    virtual MDbError DoCheckAndReconnect() = 0;
    virtual void DoClose() = 0;
    virtual MDbError DoSelectDb(const std::string &db) = 0;
    virtual MIDbCommand* DoCreateCommand() = 0;
protected:
    MDbError last_error_;
    std::string last_error_msg_;
};

#endif
