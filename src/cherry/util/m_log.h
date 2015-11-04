#ifndef _M_LOG_H_
#define _M_LOG_H_

#include <cherry/util/m_type_define.h>
#include <cherry/util/m_singleton.h>
#include <mutex>

enum class MLogLevelType
{
    Info = 0,
    Warning = 1,
    Error = 2,
    Fatal = 3,
};

typedef void (*MLogHandler) (MLogType type, const char *p_file_name, int line, const std::string &message);

class MLogFinisher;

class MLogMessage final
{
public:
    explicit MLogMessage(MLogLevelType type, const char *p_file_name, int line)
    {

    }
    MLogMessage& operator<<(const std::string& value);
    MLogMessage& operator<<(const char* value);
    MLogMessage& operator<<(char value);
    MLogMessage& operator<<(int value);
    MLogMessage& operator<<(uint value);
    MLogMessage& operator<<(long value);
    MLogMessage& operator<<(unsigned long value);
    MLogMessage& operator<<(long long value);
    MLogMessage& operator<<(unsigned long long value);
    MLogMessage& operator<<(double value);
    MLogMessage& operator<<(void* value);
    MLogMessage& operator<<(const StringPiece& value);
    MLogMessage& operator<<(const ::google::protobuf::util::Status& status);
    MLogMessage& operator<<(const uint128& value);
private:
    friend class MLogFinisher;
    void Finish();
private:
    MLogLevelType type_;
    const char *p_file_name_;
    int line_;
    std::string message_;
};

class MLogFinisher final
{
public:
    void operator=(const MLogMessage &msg)
    {
        msg.Finish();
    }
};

#endif
