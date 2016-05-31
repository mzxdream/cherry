#ifndef _SESSION_MGR_H_
#define _SESSION_MGR_H_

#include <mzx/util/m_singleton.h>
#include <map>

class SessionMgr
{
public:
    SessionMgr();
    ~SessionMgr();
    SessionMgr(const SessionMgr &) = delete;
    SessionMgr& operator=(const SessionMgr &) = delete;
public:
    bool Init();
    void Clear();
private:

};

#endif
