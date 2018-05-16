#include "packet_model.h"
#include "log.h"
#include "tools.h"

map<string, map<string, boost::shared_ptr<CDealModel>> > 
    CPacketModel::m_uriInterfaceDealModel = 
        map<string, map<string, boost::shared_ptr<CDealModel>> >();

bool CPacketModel::Pack()
{
    Json::Value jRet;
    jRet["_head"]                    = Json::Value();
    jRet["_head"]["_interface"]      = m_strInterface;
    jRet["_head"]["_timestamps"]     = m_strTimestamps;
    jRet["_head"]["_invokeId"]       = m_strInvokeId;
    jRet["_head"]["_version"]        = m_strVersion;
    jRet["_head"]["_callerServerId"] = m_strCallerServerId;
    jRet["_body"]                    = Json::Value();
    jRet["_body"]["_ret"]            = m_strRet;
    jRet["_body"]["_retCode"]        = m_strRetCode;
    jRet["_body"]["_retInfo"]        = m_strRetInfo;
    jRet["_body"]["_data"]           = m_jData;

    m_strOut = m_jFastWriter.write(jRet);
    return true;
}

bool CPacketModel::CheckAndUnpack()
{
    Reset();
    if(m_uriInterfaceDealModel.find(m_strUrlPath) == m_uriInterfaceDealModel.end())
    {
        m_strRet = "1";
        m_strRetCode = "100000";
        m_strRetInfo = m_strUrlPath + "未注册";
        ERROR("未找到url:" + m_strUrlPath);
        return false;
    }

    Json::Value jPacket;
    if(!m_jReader.parse(m_strIn, jPacket))
    {
        m_strRet = "1";
        m_strRetCode = "100001";
        m_strRetInfo = "解析报文失败";
        ERROR("解析报文失败");
        return false;
    }
    if(!jPacket.isMember("_head") 
            || !jPacket["_head"].isObject()
            || !jPacket["_head"].isMember("_interface")
            || !jPacket["_head"].isMember("_timestamps")
            || !jPacket["_head"].isMember("_invokeId")
            || !jPacket["_head"].isMember("_callerServerId")
            || !jPacket["_head"].isMember("_version")
            || !jPacket["_head"]["_interface"].isString()
            || !jPacket["_head"]["_timestamps"].isString()
            || !jPacket["_head"]["_invokeId"].isString()
            || !jPacket["_head"]["_callerServerId"].isString()
            || !jPacket["_head"]["_version"].isString()
            || !jPacket.isMember("_params")
            || !jPacket["_params"].isObject())
    {
        m_strRet = "1";
        m_strRetCode = "100002";
        m_strRetInfo = "报文头部格式错误";
        ERROR("报文头部格式错误");
        return false;
    }
    m_strInterface      = jPacket["_head"]["_interface"].asString();
    m_strTimestamps     = jPacket["_head"]["_timestamps"].asString();
    m_strInvokeId       = jPacket["_head"]["_invokeId"].asString();
    m_strVersion        = jPacket["_head"]["_version"].asString();
    m_strCallerServerId = jPacket["_head"]["_callerServerId"].asString();

    m_jParams           = jPacket["_params"];
    if(m_uriInterfaceDealModel[m_strUrlPath].find(m_strInterface) == m_uriInterfaceDealModel[m_strUrlPath].end())
    {
        m_strRet = "1";
        m_strRetCode = "100003";
        m_strRetInfo = m_strUrlPath + "下" +  m_strInterface + "未注册";
        ERROR("未找到interface:" + m_strInterface);
        return false;
    }

    return true;
}

bool CPacketModel::Deal()
{
    CDealModel *pDealModel = m_uriInterfaceDealModel[m_strUrlPath][m_strInterface].get();
    if(NULL == pDealModel)
    {
        m_strRet = "1";
        m_strRetCode = "100004";
        m_strRetInfo = "处理模块为NULL";
        ERROR("pDealModel is NULL");
        return false;
    }

    pDealModel->RealDeal(m_jParams, m_strRet, m_strRetCode,
            m_strRetInfo, m_jData);

    return true;
}

bool CPacketModel::AddInterface(const string& strUri, 
        const string &strInterface, CDealModel *pDealModel)
{
    if(strUri.empty())
    {
        ERROR("strUri is NULL");
        return false;
    }
    if(strInterface.empty())
    {
        ERROR("strInterface is NULL");
        return false;
    }
    if(NULL == pDealModel)
    {
        ERROR("pDealModel is NULL");
        return false;
    }

    DEBUG("Uri:" + strUri + " " + "Interface:" + strInterface);

    CPacketModel::m_uriInterfaceDealModel[strUri][strInterface] 
        = boost::shared_ptr<CDealModel>(pDealModel);

    return true;
}


bool CPacketModel::DealPacket()
{
    do 
    {
        if(!CheckAndUnpack())
        {
            ERROR("CheckAndUnpack failure");
            break;
        }
        if(!Deal())
        {
            ERROR("CheckAndUnpack failure");
            break;
        }
    }
    while(0);

    Pack();

    return true;
}
