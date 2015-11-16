#include <mzx/db/m_mysql_connection.h>
#include <mzx/db/m_mysql_command.h>

MMysqlCommand::MMysqlCommand(MMysqlConnection &conn)
    :conn_(conn)
{
}

MMysqlCommand::~MMysqlCommand()
{
}

