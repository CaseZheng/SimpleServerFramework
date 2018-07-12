#include <boost/shared_ptr.hpp>
#include "packet_model.h"
#include "log.h"
#include "tools.h"

void *CMessageProcess::ReqNew(void * pArg)
{
    DEBUG("new ReqMessage");
    return static_cast<void*>(new ReqMessage());
}

void CMessageProcess::ReqFree(void *pReq)
{
    DEBUG("delete ReqMessage");
    delete static_cast<ReqMessage*>(pReq);
}

//将缓存区数据转化为请求结构体的数据
int CMessageProcess::ReqUnmarshal(void *pReq, struct evbuffer *pBuffer)
{
    if(NULL == pReq)
    {
        ERROR("pReq is NULL");
        return 0;
    }
    if(NULL == pBuffer)
    {
        ERROR("pBuffer is NULL");
        return 0;
    }

    int len = evbuffer_get_length(pBuffer);
    if(len < 1)
    {
        ERROR("evbuffer lenght is error: " + to_string(len));
        return 0;
    }

    boost::shared_ptr<char> pBuff(new char[len+1]);
    if(pBuff == NULL)
    {
        ERROR("new Buff failure");
        return 0;
    }
    memset(pBuff.get(), 0, len+1);

    int copyLen = evbuffer_remove(pBuffer, pBuff.get(), len);
    if(copyLen < len)
    {
        ERROR("evbuffer_remove copyLen < len");
        return 0;
    }
    DEBUG("copyLen:" + to_string(copyLen) + " len:" + to_string(len) + " info:" + string(pBuff.get()));

    Json::Value jInfo;
    if(!Json::Reader(Json::Features::strictMode()).parse(pBuff.get(), jInfo))
    {
        ERROR("Json parse error");
        return 0;
    }

    if(!jInfo.isMember("_head") || !jInfo["_head"].isObject())
    {
        ERROR("Json _head is failure");
        return 0;
    }

    Json::Value &jHead = jInfo["_head"];
    if(!jHead.isMember("_timestamps") || !jHead["_timestamps"].isString()
            || !jHead.isMember("_invokeId") || !jHead["_invokeId"].isString()
            || !jHead.isMember("_version") || !jHead["_version"].isString()
            || !jHead.isMember("_callerServerId") || !jHead["_callerServerId"].isString())
    {
        ERROR("Json _head is failure");
        return 0;
    }

    if(!jInfo.isMember("_param") || !jInfo["_param"].isObject())
    {
        ERROR("Json _param is failure");
        return 0;
    }

    ReqMessage *pReqMessage = static_cast<ReqMessage*>(pReq);
    pReqMessage->m_strTimestamps     = jHead["_timestamps"].asString();
    pReqMessage->m_strInvokeId       = jHead["_invokeId"].asString();
    pReqMessage->m_strVersion        = jHead["_version"].asString();
    pReqMessage->m_strCallerServerId = jHead["_callerServerId"].asString();
    pReqMessage->m_jParam            = jInfo["_param"];

    DEBUG("---------------------------");

    return 0;
}

void *CMessageProcess::RplNew(void *pArg)
{
    DEBUG("new RplMessage");
    return static_cast<void*>(new RplMessage());
}

void CMessageProcess::RplFree(void *pRpl)
{
    DEBUG("delete RplMessage");
    delete static_cast<RplMessage*>(pRpl);
}

//验证返回是否有效
int CMessageProcess::RplComplete(void *pRpl)
{
    //RplMessage *pRplMessage = static_cast<RplMessage*>(pRpl);
    DEBUG("RplComplete");
    return 0;
}

//将返回结构体转化为缓存区信息
void CMessageProcess::RplMarshal(struct evbuffer *pBuffer, void *pRpl)
{
    RplMessage *pRplMessage = static_cast<RplMessage*>(pRpl);

    Json::Value jInfo;
    Json::Value &jHead       = jInfo["_head"];
    jHead["_timestamps"]     = pRplMessage->m_strTimestamps;
    jHead["_invokeId"]       = pRplMessage->m_strInvokeId;
    jHead["_version"]        = pRplMessage->m_strVersion;
    jHead["_callerServerId"] = pRplMessage->m_strCallerServerId;

    Json::Value &jBody       = jInfo["_body"];
    jBody["_ret"]            = pRplMessage->m_strRet;
    jBody["_errCode"]        = pRplMessage->m_strErrorCode;
    jBody["_errMsg"]         = pRplMessage->m_strErrorMsg;
    jBody["_data"]           = pRplMessage->m_jData;

    string strData = Json::FastWriter().write(jInfo);
    evbuffer_add(pBuffer, static_cast<const void*>(strData.c_str()), strData.length());
}
