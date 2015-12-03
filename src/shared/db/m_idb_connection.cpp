#include <db/m_idb_connection.h>

MDbConnParamStyleType MIDbConnection::GetParamStyleType()
{
    return DoGetParamStyleType();
}

MDbConnThreadSafetyType MIDbConnection::GetThreadSafetyType()
{
    return DoGetThreadSafetyType();
}

MDbError MIDbConnection::Open(const std::string &conn_string)
{
    return DoOpen(conn_string);
}

MDbError MIDbConnection::CheckConnect()
{
    return DoCheckConnect();
}

MDbError MIDbConnection::CheckAndReConnect()
{
    return DoCheckAndReconnect();
}

void MIDbConnection::Close()
{
    DoClose();
}

MDbError MIDbConnection::SelectDb(const std::string &db)
{
    return DoSelectDb(db);
}

MIDbCommand* MIDbConnection::CreateCommand()
{
    return DoCreateCommand();
}

MDbError MIDbConnection::GetLastError()
{
    return DoGetLastError();
}

std::string MIDbConnection::GetLastErrorMsg()
{
    return DoGetLastErrorMsg();
}

