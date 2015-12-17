#ifndef _M_NET_CONNECTOR_H_
#define _M_NET_CONNECTOR_H_

#include <net/m_socket.h>

class MNetListener;

class MNetConnector
{
public:
    MNetConnector();
    ~MNetConnector();
    MNetConnector(const MNetConnector &) = delete;
    MNetConnector& operator=(const MNetConnector &) = delete;
public:
private:
    MSocket sock_;
    MNetListener &listener_;
};

#endif


