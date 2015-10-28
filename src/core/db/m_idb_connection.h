#ifndef _M_IDB_CONNECTION_H_
#define _M_IDB_CONNECTION_H_

#include <string>

enum class DbConnThreadSafetyType
{
    None = 0,
    Junior = 1,
    Medium = 2,
    Senior = 3,
};

enum class DbConnParamStyleType
{
    Format = 0,
    QuestionMark = 1,
};

class MIDbConnection
{
pubic:
    MIDbConnection() = default;
    virtual ~MIDbConnection() = default;
    MIDbConnection(const MIDbConnection &) = delete;
    MIDbConnection& operator=(const MIDbConnection &) = delete;
public:
    DbConnParamStyleType GetParamStyleType();
    DbConnThreadSafetyType GetThreadSafetyType();
    bool Open(const std::string &conn_string);
    void Close();
    bool StartTran();
    bool CommitTran();
    bool RollBackTran();
    char* CreateCursor();
};

#endif
