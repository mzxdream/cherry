#include "app_conf.h"
#include <rapidjson/document.h>
#include <sstream>
#include <iostream>
#include <fstream>

AppConf::AppConf()
{
}

AppConf::~AppConf()
{
}

bool AppConf::Init(const std::string &conf)
{
    std::stringstream ss;
    std::ifstream in(conf, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "open " << conf << " failed" << std::endl;
        return false;
    }
    ss << in.rdbuf();
    in.close();

    rapidjson::Document doc;
    if (doc.Parse<0>(ss.str().c_str()).HasParseError())
    {
        std::cerr << "parse conf:" << conf << std::endl;
        return false;
    }
    ip_ = doc["ip"].GetString();
    port_ = doc["port"].GetUint();
    client_ip_ = doc["client_ip"].GetString();
    client_port_ = doc["client_port"].GetUint();
    return true;
}

void AppConf::Clear()
{
}

const std::string& AppConf::GetIP() const
{
    return ip_;
}

unsigned AppConf::GetPort() const
{
    return port_;
}

const std::string& AppConf::GetClientIP() const
{
    return client_ip_;
}

unsigned AppConf::GetClientPort() const
{
    return client_port_;
}
