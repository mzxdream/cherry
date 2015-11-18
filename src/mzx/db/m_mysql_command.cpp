#include <mzx/db/m_mysql_connection.h>
#include <mzx/db/m_mysql_command.h>

MMysqlCommand::MMysqlCommand(MMysqlConnection &conn)
    :conn_(conn)
    ,p_stmt_(nullptr)
{
}

MMysqlCommand::~MMysqlCommand()
{
    if (p_stmt_)
    {
        mysql_stmt_close(p_stmt_);
        p_stmt_ = nullptr;
    }
}

bool MMysqlCommand::DoPrepair(const std::string &command)
{
    if (p_stmt_)
    {
        mysql_stmt_close(p_stmt_);
        p_stmt_ = nullptr;
    }
    p_stmt_ = mysql_stmt_init(conn_.GetConnection());
    if (!p_stmt_)
    {
        MLOG(Error) << "init stmt failed errorno:" << mysql_errno(conn_.GetConnection()) << "error:" << mysql_error(conn_.GetConnection());
        return false;
    }
    if (mysql_stmt_prepare(p_stmt_, command.c_str(), command.size()) != 0)
    {
        MLOG(Error) << "mysql prepare " << command << " failed error:" << mysql_stmt_error(p_stmt_);
        return false;
    }
    return true;
}

bool MMysqlCommand::DoBeforeAddParam()
{
    params_.clear();
    return true;
}

int MMysqlCommand::DoExecuteNonQuery()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (param_count != params_.size())
    {
        MLOG(Error) << "param count is not match need:" << param_count << " actual is :" << params_.size();
        return 0;
    }
    if (mysql_stmt_bind_param(p_stmt_, &params_[0]) != 0)
    {
        MLOG(Error) << "bind failed error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        MLOG(Error) << "execute failed error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }
    return static_cast<int>(mysql_stmt_affected_rows(p_stmt_));
}

bool MMysqlCommand::DoExecuteReader()
{
    int param_count = mysql_stmt_param_count(p_stmt_);
    if (param_count != params_.size())
    {
        MLOG(Error) << "param count is not match need:" << param_count << " actual is :" << params_.size();
        return 0;
    }
    if (mysql_stmt_bind_param(p_stmt_, &params_[0]) != 0)
    {
        MLOG(Error) << "bind failed error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }
    if (mysql_stmt_execute(p_stmt_) != 0)
    {
        MLOG(Error) << "execute failed error:" << mysql_stmt_error(p_stmt_);
        return 0;
    }

}
