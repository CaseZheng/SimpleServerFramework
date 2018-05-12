#include "packet_model.h"
#include "log.h"

map<string, map<string, boost::shared_ptr<CDealModel>> > 
    CPacketModel::m_uriInterfaceDealModel = 
        map<string, map<string, boost::shared_ptr<CDealModel>> >();

void CPacketModel::InitInPacket(const enum evhttp_cmd_type eHttpType,
        const std::string& strIn,
        const std::string& strUrlPath, 
        const std::string& strUrlParams)
{
    Reset();

    m_eHttpType    = eHttpType;
    m_strIn        = strIn;
    m_strUrlPath   = strUrlPath;
    m_strUrlParams = strUrlParams;
}

void CPacketModel::DealPacket()
{
    if(!CheckAndUnpack())
    {
        ERROR("CheckAndUnpack failure");
        return;
    }
    if(!Deal())
    {
        ERROR("CheckAndUnpack failure");
        return;
    }

    return;
}

bool CPacketModel::CheckAndUnpack()
{
    return true;
}

bool CPacketModel::Deal()
{
    m_strRespCode = "200";
    m_strOut = "hello example";
    m_strReason = "hello reason";
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

    CPacketModel::m_uriInterfaceDealModel[strUri][strInterface] 
        = boost::shared_ptr<CDealModel>(pDealModel);

    return true;
}
