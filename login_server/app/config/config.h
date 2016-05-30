#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <mzx/util/m_singleton.h>
#include <string>

class Config
    :public MSingleton<Config>
{
protected:
    Config();
    ~Config();
    Config(const Config &) = delete;
    Config& operator=(const Config &) = delete;
    friend class MSingleton<Config>;
public:
    bool Init(const std::string &conf);
    void Clear();
};

#endif
