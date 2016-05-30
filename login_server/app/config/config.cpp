#include "config.h"
#include "sys/app_conf.h"
#include <iostream>

Config::Config()
{
}

Config::~Config()
{
}

bool Config::Init(const std::string &conf)
{
    if (!AppConf::Instance().Init(conf+"/sys/app.json"))
    {
        std::cerr << "init app config failed" << std::endl;
        return false;
    }
    return true;
}

void Config::Clear()
{
    AppConf::Instance().Clear();
}
