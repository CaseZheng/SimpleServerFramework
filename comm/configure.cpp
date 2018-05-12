#include "configure.h"

string CConfigure::m_strId      = "";
string CConfigure::m_strIp      = "";
string CConfigure::m_strPort    = "";
string CConfigure::m_strMySqlh  = "";
string CConfigure::m_strMySqlP  = "";
string CConfigure::m_strMySqlu  = "";
string CConfigure::m_strMySqlp  = "";
string CConfigure::m_strRedish  = "";
string CConfigure::m_strRedisp  = "";
string CConfigure::m_strRedisa  = "";
string CConfigure::m_strLogPath = "";


bool CConfigure::InitConf(const string &strConfPath)
{
    boost::property_tree::ptree xmlTree;
    try
    {
    read_xml(strConfPath, xmlTree);

    //m_strIp    = xmlTree.get<string>("xml.server.ip");   //如果xml.server.ip不存在 则抛出异常
    m_strId      = xmlTree.get("xml.server.id", "");
    m_strIp      = xmlTree.get("xml.server.ip", "");         //如果xml.server.ip不存在 则默认为""
    m_strPort    = xmlTree.get("xml.server.port", "");
    m_strMySqlh  = xmlTree.get("xml.mysql.host", "");
    m_strMySqlP  = xmlTree.get("xml.mysql.port", "");
    m_strMySqlu  = xmlTree.get("xml.mysql.username", "");
    m_strMySqlp  = xmlTree.get("xml.mysql.passwd", "");
    m_strRedish  = xmlTree.get("xml.redis.host", "");
    m_strRedisp  = xmlTree.get("xml.redis.port", "");
    m_strRedisa  = xmlTree.get("xml.redis.auth", "");
    m_strLogPath = xmlTree.get("xml.logpath", "./log/");
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
    DEBUG( "Id:            " + CConfigure::GetServerId());
    DEBUG( "Ip:            " + CConfigure::GetIp());
    DEBUG( "Port:          " + CConfigure::GetPort());
    DEBUG( "MySqlHost:     " + CConfigure::GetMySqlHost());
    DEBUG( "MySqlPort:     " + CConfigure::GetMySqlPort());
    DEBUG( "MySqlUserName: " + CConfigure::GetMySqlUserName());
    DEBUG( "MySqlPasswd:   " + CConfigure::GetMySqlPasswd());
    DEBUG( "RedisHost:     " + CConfigure::GetRedisHost());
    DEBUG( "RedisPort:     " + CConfigure::GetRedisPort());
    DEBUG( "RedisAuth:     " + CConfigure::GetRedisAuth());
    DEBUG( "LogPath:       " + CConfigure::GetLogPath());
}
