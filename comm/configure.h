#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <exception>

#include "log.h"

using namespace std;

struct ServiceInfo 
{
    string m_strId;
    string m_strIp;
    string m_strPort;
};

class CConfigure
{
public:
    static bool InitConf(const string &strConfPath);
    static void PrintConf();

    static const string GetServiceId(const string & strServiceName="")
    {
        string strName = strServiceName;
        if(strName.empty())
        {
            strName = tools::GetProcessName();
        }
        if(m_mapServiceInfo.find(strName) != m_mapServiceInfo.end())
        {
            return m_mapServiceInfo[strName].m_strId;
        }
        return "";
    }
    static const string GetIp(const string & strServiceName="")
    {
        string strName = strServiceName;
        if(strName.empty())
        {
            strName = tools::GetProcessName();
        }

        if(m_mapServiceInfo.find(strName) != m_mapServiceInfo.end())
        {
            return m_mapServiceInfo[strName].m_strIp;
        }
        return "";
    }
    static const string GetPort(const string & strServiceName="")
    {
        string strName = strServiceName;
        if(strName.empty())
        {
            strName = tools::GetProcessName();
        }
        if(m_mapServiceInfo.find(strName) != m_mapServiceInfo.end())
        {
            return m_mapServiceInfo[strName].m_strPort;
        }
        return "";
    }

    static const string & GetMySqlHost()        { return m_strMySqlh; }
    static const string & GetMySqlPort()        { return m_strMySqlP; }
    static const string & GetMySqlUserName()    { return m_strMySqlu; }
    static const string & GetMySqlPasswd()      { return m_strMySqlp; }

    static const string & GetRedisHost()        { return m_strRedish; }
    static const string & GetRedisPort()        { return m_strRedisp; }
    static const string & GetRedisAuth()        { return m_strRedisa; }

    static const string & GetRabbitMqHost()     { return m_strRabbitMQh; }
    static const string & GetRabbitMqPort()     { return m_strRabbitMQP; }
    static const string & GetRabbitMqUserName() { return m_strRabbitMQu; }
    static const string & GetRabbitMqPasswd()   { return m_strRabbitMQp; }

    static const string & GetKafkaHostPort()    { return m_strKafkaHostPort; }

    static const string & GetLogPath()          { return m_strLogPath;}

private:
    CConfigure(const CConfigure&);
    CConfigure &operator=(const CConfigure&);

    static map<string, ServiceInfo> m_mapServiceInfo; // <ServiceName, ServiceInfo>
    static string m_strMySqlh;                        // mysql host
    static string m_strMySqlP;                        // mysql port
    static string m_strMySqlu;                        // mysql username
    static string m_strMySqlp;                        // mysql passwd
    static string m_strRedish;                        // redis host
    static string m_strRedisp;                        // redis port
    static string m_strRedisa;                        // redis auth
    static string m_strRabbitMQh;                     // rabbitMq host
    static string m_strRabbitMQP;                     // rabbitMq port
    static string m_strRabbitMQu;                     // rabbitMq username
    static string m_strRabbitMQp;                     // rabbitMq password
    static string m_strKafkaHostPort;                 // kafka host port
    static string m_strLogPath;                       // log path
};

#endif
