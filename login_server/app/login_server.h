#ifndef _LOGIN_SERVER_H_
#define _LOGIN_SERVER_H_

#include <mzx/util/m_singleton.h>

class LoginServer
    :public MSingleton<LoginServer>
{
protected:
    LoginServer();
    ~LoginServer();
    LoginServer(const LoginServer &) = delete;
    LoginServer& operator=(const LoginServer &) = delete;
    friend class MSingleton<LoginServer>;
public:
    bool Init(const std::string &conf);
    void Clear();

    bool Update();
private:
    bool InitConf(const std::string &conf);
    void ClearConf();
};

#endif
