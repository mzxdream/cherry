#ifndef _M_IDB_CONNECTION_HPP_
#define _M_IDB_CONNECTION_HPP_

#include <string>

class MDbParam
{
pubic:
    enum ParamType{
        ParamTypeNull = 0,
        ParamTypeInt = 1,
    };
public:
    MDbParam()
    {
    }
    ~MDbParam()
    {
    }
public:
    ParamType type_;
    void *p_buf_;
    unsigned long buf_length_; 
};

class MIDbConnection
{
pubic:
    MIDbConnection(){}
    virtual ~MIDbConnection(){}
private:
    MIDbConnection(const MIDbConnection &);
    MIDbConnection& operator=(const MIDbConnection &);
public:
    virtual bool Open(const std::string &connection_string) = 0;
    virtual void Close() = 0;
    virtual bool Prepare(const std::string &query) = 0; 
    virtual bool 
};

#endif
