#include "configure.h"
#include "boost/typeof/typeof.hpp"

string CConfigure::m_strMySqlh        = "";
string CConfigure::m_strMySqlP        = "";
string CConfigure::m_strMySqlu        = "";
string CConfigure::m_strMySqlp        = "";
string CConfigure::m_strRedish        = "";
string CConfigure::m_strRedisp        = "";
string CConfigure::m_strRedisa        = "";
string CConfigure::m_strLogPath       = "";
string CConfigure::m_strRabbitMQh     = "";
string CConfigure::m_strRabbitMQP     = "";
string CConfigure::m_strRabbitMQu     = "";
string CConfigure::m_strRabbitMQp     = "";
string CConfigure::m_strKafkaHostPort = "";
map<string, ServiceInfo> CConfigure::m_mapServiceInfo;

bool CConfigure::InitConf(const string &strConfPath)
{
    string strTmpConfPath = strConfPath;
    boost::property_tree::ptree xmlTree;
    try
    {
    read_xml(strTmpConfPath, xmlTree);

    //m_strIp          = xmlTree.get<string>("xml.mysql.host");   //如果xml.service.ip不存在 则抛出异常
    m_strMySqlh        = xmlTree.get("xml.mysql.host", "");
    m_strMySqlP        = xmlTree.get("xml.mysql.port", "");
    m_strMySqlu        = xmlTree.get("xml.mysql.username", "");
    m_strMySqlp        = xmlTree.get("xml.mysql.passwd", "");
    m_strRedish        = xmlTree.get("xml.redis.host", "");
    m_strRedisp        = xmlTree.get("xml.redis.port", "");
    m_strRedisa        = xmlTree.get("xml.redis.auth", "");
    m_strRabbitMQh     = xmlTree.get("xml.rabbitmq.host", "");
    m_strRabbitMQP     = xmlTree.get("xml.rabbitmq.port", "");
    m_strRabbitMQu     = xmlTree.get("xml.rabbitmq.username", "");
    m_strRabbitMQp     = xmlTree.get("xml.rabbitmq.passwd", "");
    m_strKafkaHostPort = xmlTree.get("xml.kafka.hostport", "");
    m_strLogPath       = xmlTree.get("xml.logpath", "./log/");
    BOOST_AUTO(arrayTree, xmlTree.get_child("xml"));
    for(BOOST_AUTO(it, arrayTree.begin()); it!=arrayTree.end(); ++it)
    {
        if(it->first == "service")
        {
            string strServiceName = it->second.get("name", ""); 
            if(strServiceName.empty())
            {
                continue;
            }
            ServiceInfo sInfo;
            sInfo.m_strId   = it->second.get("id", "-1");
            sInfo.m_strIp   = it->second.get("ip", "127.0.0.1");
            sInfo.m_strPort = it->second.get("port", "0");
            m_mapServiceInfo[strServiceName] = sInfo;
        }
    }
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
    for(auto &it : m_mapServiceInfo)
    {
        DEBUG( "ServiceName:" << it.first 
                << "ServiceId:" << it.second.m_strId
                << "ServiceIp:" << it.second.m_strIp
                << "ServicePort:" << it.second.m_strPort);
    }

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
