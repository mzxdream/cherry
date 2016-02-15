#ifndef _LOGIN_SERVER_H_
#define _LOGIN_SERVER_H_

#include <util/m_singleton.h>
#include <mutex>

class LoginServer
    :public MSingleton<LoginServer>
{
public:
    bool Init();
    void Clear();
    bool Update();
private:
    bool LoadConf();
};

#endif
