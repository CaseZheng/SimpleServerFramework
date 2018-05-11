#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <exception>

#include "log.h"

using namespace std;

class CConfigure
{
public:
    static bool InitConf(const string &strConfPath);
    static void PrintConf();

    static const string & GetIp()            { return m_strIp; }
    static const string & GetPort()          { return m_strPort; }
    static const string & GetMySqlHost()     { return m_strMySqlh; }
    static const string & GetMySqlPort()     { return m_strMySqlP; }
    static const string & GetMySqlUserName() { return m_strMySqlu; }
    static const string & GetMySqlPasswd()   { return m_strMySqlp; }
    static const string & GetRedisHost()     { return m_strRedish; }
    static const string & GetRedisPort()     { return m_strRedisp; }
    static const string & GetRedisAuth()     { return m_strRedisa; }
private:
    CConfigure(const CConfigure&);
    CConfigure &operator=(const CConfigure&);

    static string m_strIp;            //listen ip
    static string m_strPort;          //listen port

    static string m_strMySqlh;        //mysql host
    static string m_strMySqlP;        //mysql port
    static string m_strMySqlu;        //mysql username
    static string m_strMySqlp;        //mysql passwd

    static string m_strRedish;        //redis host
    static string m_strRedisp;        //redis port
    static string m_strRedisa;        //redis auth
};

string CConfigure::m_strIp     = "";
string CConfigure::m_strPort   = "";
string CConfigure::m_strMySqlh = "";
string CConfigure::m_strMySqlP = "";
string CConfigure::m_strMySqlu = "";
string CConfigure::m_strMySqlp = "";
string CConfigure::m_strRedish = "";
string CConfigure::m_strRedisp = "";
string CConfigure::m_strRedisa = "";

bool CConfigure::InitConf(const string &strConfPath)
{
    boost::property_tree::ptree xmlTree;
    try
    {
    read_xml(strConfPath, xmlTree);

    //m_strIp     = xmlTree.get<string>("xml.server.ip");   //如果xml.server.ip不存在 则抛出异常
    m_strIp     = xmlTree.get("xml.server.ip", "");         //如果xml.server.ip不存在 则默认为""
    m_strPort   = xmlTree.get("xml.server.port", "");
    m_strMySqlh = xmlTree.get("xml.mysql.host", "");
    m_strMySqlP = xmlTree.get("xml.mysql.port", "");
    m_strMySqlu = xmlTree.get("xml.mysql.username", "");
    m_strMySqlp = xmlTree.get("xml.mysql.passwd", "");
    m_strRedish = xmlTree.get("xml.redis.host", "");
    m_strRedisp = xmlTree.get("xml.redis.port", "");
    m_strRedisa = xmlTree.get("xml.redis.auth", "");
    }
    catch (std::exception& e)  
    {  
        DEBUG(e.what());
        return false;
    }  

    return true;
}

void CConfigure::PrintConf()
{

    DEBUG( "Ip:            " + CConfigure::GetIp());
    DEBUG( "Port:          " + CConfigure::GetPort());
    DEBUG( "MySqlHost:     " + CConfigure::GetMySqlHost());
    DEBUG( "MySqlPort:     " + CConfigure::GetMySqlPort());
    DEBUG( "MySqlUserName: " + CConfigure::GetMySqlUserName());
    DEBUG( "MySqlPasswd:   " + CConfigure::GetMySqlPasswd());
    DEBUG( "RedisHost:     " + CConfigure::GetRedisHost());
    DEBUG( "RedisPort:     " + CConfigure::GetRedisPort());
    DEBUG( "RedisAuth:     " + CConfigure::GetRedisAuth());
}

#endif
