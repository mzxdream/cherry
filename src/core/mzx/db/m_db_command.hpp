#ifndef _M_DB_COMMAND_HPP_
#define _M_DB_COMMAND_HPP_

#include "m_idb_connection.hpp"

class MDbCommand
{
public:
    explicit MDbCommand(MIDbConnection &connection)
        :connection_(connection)
    {
    }
    ~MDbCommand()
    {
    }
private:
    MDbCommand(const MDbCommand &);
    MDbCommand& operator=(const MDbCommand &);
public:
private:
    MIDbConnection &connection_;
};

#endif
