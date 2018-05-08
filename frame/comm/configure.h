#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include "log.h"

using namespace std;

class CConfigure
{
public:
    static bool InitConf(const string &strConfPath);
    static void PrintConf();
private:
    static string m_strIp;            //listen ip
    static string m_strPort;          //listen port

    static string m_strMySqlh;        //mysql host
    static string m_strMySqlP;        //mysql port
    static string m_strMySqlu;        //mysql username
    static string m_strMySqlp;        //mysql passwd

    static string m_strRedish;        //redis host
    static string m_strRedisp;        //redis port
    static string m_strRedisa;        //reids auth
};

string CConfigure::m_strIp = "";
string CConfigure::m_strPort = "";
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
    read_xml(strConfPath, xmlTree);
    m_strIp   = xmlTree.get<std::string>("xml.server.ip");
    m_strPort = xmlTree.get<std::string>("xml.server.port");

    return true;
}

void CConfigure::PrintConf()
{
    DEBUG("Ip:  " + m_strIp);
    DEBUG("Port:" + m_strPort);
}

#endif
