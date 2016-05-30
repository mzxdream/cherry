#include "login_server.h"
#include "config/config.h"
#include <iostream>
#include "config/sys/app_conf.h"

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{
}

bool LoginServer::Init(const std::string &conf)
{
    if (!Config::Instance().Init(conf))
    {
        return false;
    }
    std::cout << AppConf::Instance().GetIP() << std::endl;
    std::cout << AppConf::Instance().GetPort() << std::endl;
    std::cout << AppConf::Instance().GetClientIP() << std::endl;
    std::cout << AppConf::Instance().GetClientPort() << std::endl;
    return true;
}

void LoginServer::Clear()
{
    Config::Instance().Clear();
}

bool LoginServer::Update()
{
    return true;
}
