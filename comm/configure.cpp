#include "configure.h"

string CConfigure::m_strId        = "";
string CConfigure::m_strIp        = "";
string CConfigure::m_strPort      = "";
string CConfigure::m_strMySqlh    = "";
string CConfigure::m_strMySqlP    = "";
string CConfigure::m_strMySqlu    = "";
string CConfigure::m_strMySqlp    = "";
string CConfigure::m_strRedish    = "";
string CConfigure::m_strRedisp    = "";
string CConfigure::m_strRedisa    = "";
string CConfigure::m_strLogPath   = "";
string CConfigure::m_strRabbitMQh = "";
string CConfigure::m_strRabbitMQP = "";
string CConfigure::m_strRabbitMQu = "";
string CConfigure::m_strRabbitMQp = "";

bool CConfigure::InitConf(const string &strConfPath)
{
    string strTmpConfPath = strConfPath;
    if(strTmpConfPath.empty())
    {
        strTmpConfPath = "/data/config/config.xml";
    }
    boost::property_tree::ptree xmlTree;
    try
    {
    read_xml(strTmpConfPath, xmlTree);

    //m_strIp      = xmlTree.get<string>("xml.server.ip");   //如果xml.server.ip不存在 则抛出异常
    m_strId        = xmlTree.get("xml.server.id", "");
    m_strIp        = xmlTree.get("xml.server.ip", "");         //如果xml.server.ip不存在 则默认为""
    m_strPort      = xmlTree.get("xml.server.port", "");
    m_strMySqlh    = xmlTree.get("xml.mysql.host", "");
    m_strMySqlP    = xmlTree.get("xml.mysql.port", "");
    m_strMySqlu    = xmlTree.get("xml.mysql.username", "");
    m_strMySqlp    = xmlTree.get("xml.mysql.passwd", "");
    m_strRedish    = xmlTree.get("xml.redis.host", "");
    m_strRedisp    = xmlTree.get("xml.redis.port", "");
    m_strRedisa    = xmlTree.get("xml.redis.auth", "");
    m_strRabbitMQh = xmlTree.get("xml.rabbitmq.host", "");
    m_strRabbitMQP = xmlTree.get("xml.rabbitmq.port", "");
    m_strRabbitMQu = xmlTree.get("xml.rabbitmq.username", "");
    m_strRabbitMQp = xmlTree.get("xml.rabbitmq.passwd", "");
    m_strLogPath   = xmlTree.get("xml.logpath", "./log/");
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
    DEBUG( "Id:"               + CConfigure::GetServerId());
    DEBUG( "Ip:"               + CConfigure::GetIp());
    DEBUG( "Port:"             + CConfigure::GetPort());

    DEBUG( "MySqlHost:"        + CConfigure::GetMySqlHost());
    DEBUG( "MySqlPort:"        + CConfigure::GetMySqlPort());
    DEBUG( "MySqlUserName:"    + CConfigure::GetMySqlUserName());
    DEBUG( "MySqlPasswd:"      + CConfigure::GetMySqlPasswd());

    DEBUG( "RedisHost:"        + CConfigure::GetRedisHost());
    DEBUG( "RedisPort:"        + CConfigure::GetRedisPort());
    DEBUG( "RedisAuth:"        + CConfigure::GetRedisAuth());

    DEBUG( "RabbitMQHost:"     + CConfigure::GetRabbitMqHost());
    DEBUG( "RabbitMQPort:"     + CConfigure::GetRabbitMqPort());
    DEBUG( "RabbitMQUserName:" + CConfigure::GetRabbitMqUserName());
    DEBUG( "RabbitMQPassword:" + CConfigure::GetRabbitMqPasswd());

    DEBUG( "LogPath:"          + CConfigure::GetLogPath());
}
