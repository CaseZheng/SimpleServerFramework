#ifndef _PACKET_MODEL_H_
#define _PACKET_MODEL_H_

#include <string>
#include <map>
#include <evhttp.h>
#include <boost/shared_ptr.hpp>
#include <json/json.h>

#include "custom_packet_model.h"
#include "deal_model.h"

using namespace std;

class CPacketModel : public CPacket
{
public:
    CPacketModel(){}
    virtual ~CPacketModel(){}

    static bool AddInterface(const string& strUri, const string &strInterface, CDealModel *pDealModel);

    virtual bool DealPacket();

private:

    static map<string, map<string, boost::shared_ptr<CDealModel>> > m_uriInterfaceDealModel;

    void Reset()
    {
        m_strInterface.clear();      
        m_strTimestamps.clear();     
        m_strInvokeId.clear();       
        m_strVersion.clear();        
        m_strCallerServerId.clear(); 

        m_strRet = "0";
        m_strRetCode = "0";
        m_strRetInfo.clear();

        m_jParams.clear();
        m_jData.clear();
    }

    bool CheckAndUnpack();
    bool Deal();
    bool Pack();

    string m_strInterface;      //接口名
    string m_strTimestamps;     //时间戳
    string m_strInvokeId;       //调用唯一Id
    string m_strVersion;        //版本
    string m_strCallerServerId; //调用方ServerId

    string m_strRet;
    string m_strRetCode;
    string m_strRetInfo;

    Json::Value m_jParams;
    Json::Value m_jData;

    Json::Reader m_jReader;
    Json::FastWriter m_jFastWriter;
};

#endif
