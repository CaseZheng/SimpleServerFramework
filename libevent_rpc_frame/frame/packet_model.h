#ifndef _PACKET_MODEL_H_
#define _PACKET_MODEL_H_

#include <string>
#include <map>
#include <evhttp.h>
#include <json/json.h>

//#include "deal_model.h"

using namespace std;

struct CommonMessage
{
    CommonMessage() : m_strTimestamps(""), 
            m_strInvokeId(""), 
            m_strVersion(""),
            m_strCallerServerId("")
    {
    }
    string m_strTimestamps;     // 时间戳
    string m_strInvokeId;       // 调用唯一Id
    string m_strVersion;        // 版本
    string m_strCallerServerId; // 调用方ServerId
};

struct ReqMessage : public CommonMessage
{
    ReqMessage():CommonMessage(), m_jParam(Json::Value())
    {
    }
    Json::Value m_jParam;
};

struct RplMessage : public CommonMessage
{
    RplMessage():CommonMessage(),
            m_strRet(""),
            m_strErrorCode(""),
            m_strErrorMsg(""),
            m_jData(Json::Value())
    {
    }
    string m_strRet;
    string m_strErrorCode;
    string m_strErrorMsg;
    Json::Value m_jData;
};

class CMessageProcess
{
public:
    static void *ReqNew(void *pArg);
    static void ReqFree(void *pReq);
    static int ReqUnmarshal(void *pReq, struct evbuffer *pBuffer);
    static void *RplNew(void *pArg);
    static void RplFree(void *pRpl);
    static int RplComplete(void *pRpl);
    static void RplMarshal(struct evbuffer *pBuffer, void *pRpl);
};

#endif
