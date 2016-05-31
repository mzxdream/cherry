#include "login_server.h"
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
    if (!InitConf(conf))
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
    ClearConf();
}

bool LoginServer::Update()
{
    return true;
}

bool LoginServer::InitConf(const std::string &conf)
{
    if (!AppConf::Instance().Init(conf+"/sys/app.json"))
    {
        std::cerr << "init app config failed" << std::endl;
        return false;
    }
    return true;
}

void LoginServer::ClearConf()
{
    AppConf::Instance().Clear();
}
