#ifndef _APP_CONF_H_
#define _APP_CONF_H_

#include <string>
#include <mzx/util/m_singleton.h>

class AppConf
    :public MSingleton<AppConf>
{
protected:
    AppConf();
    ~AppConf();
    AppConf(const AppConf &) = delete;
    AppConf& operator=(const AppConf &) = delete;
    friend class MSingleton<AppConf>;
public:
    bool Init(const std::string &conf);
    void Clear();

    const std::string& GetIP() const;
    unsigned GetPort() const;
    const std::string& GetClientIP() const;
    unsigned GetClientPort() const;
private:
    std::string ip_;
    unsigned port_;
    std::string client_ip_;
    unsigned client_port_;
};

#endif
