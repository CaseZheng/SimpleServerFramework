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

    static const string & GetServerId()      { return m_strId;}
    static const string & GetIp()            { return m_strIp; }
    static const string & GetPort()          { return m_strPort; }
    static const string & GetMySqlHost()     { return m_strMySqlh; }
    static const string & GetMySqlPort()     { return m_strMySqlP; }
    static const string & GetMySqlUserName() { return m_strMySqlu; }
    static const string & GetMySqlPasswd()   { return m_strMySqlp; }
    static const string & GetRedisHost()     { return m_strRedish; }
    static const string & GetRedisPort()     { return m_strRedisp; }
    static const string & GetRedisAuth()     { return m_strRedisa; }
    static const string & GetLogPath()       { return m_strLogPath;}
private:
    CConfigure(const CConfigure&);
    CConfigure &operator=(const CConfigure&);

    static string m_strId;            //server Id
    static string m_strIp;            //listen ip
    static string m_strPort;          //listen port

    static string m_strMySqlh;        //mysql host
    static string m_strMySqlP;        //mysql port
    static string m_strMySqlu;        //mysql username
    static string m_strMySqlp;        //mysql passwd

    static string m_strRedish;        //redis host
    static string m_strRedisp;        //redis port
    static string m_strRedisa;        //redis auth

    static string m_strLogPath;       //log path
};


#endif
