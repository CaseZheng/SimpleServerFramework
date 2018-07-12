#ifndef _DEAL_MODEL_H_
#define _DEAL_MODEL_H_ 

#include <string>
#include <json/json.h>
#include <event2/rpc.h>
#include "log.h"
#include "tools.h"
#include "configure.h"
#include "packet_model.h"

using namespace std;

class CLogic 
{
public:
    static void DealModel(struct evrpc_req_generic *pReqGeneric, void *pArg)
    {
        DEBUG("DealModel");
        if(NULL == pReqGeneric)
        {
            ERROR("evrpc_req_generic is NULL");
            return;
        }
        if(NULL == pArg)
        {
            ERROR("pArg is NULL");
            return;
        }
        ReqMessage *pReq = static_cast<ReqMessage*>(evrpc_get_request(pReqGeneric));
        RplMessage *pRpl = static_cast<RplMessage*>(evrpc_get_reply(pReqGeneric));
        if(NULL == pReq || NULL == pRpl)
        {
            ERROR("pReq or pRpl is NULL");
            return;
        }

        DEBUG("Timestamps:" + pReq->m_strTimestamps + " InvokeId:" + pReq->m_strInvokeId
                + " Version:" + pReq->m_strVersion + " CallerServerId:" + pReq->m_strCallerServerId
                + " Param:" + Json::FastWriter().write(pReq->m_jParam));

        CLogic *pLogic = static_cast<CLogic*>(pArg);
        if(!pLogic->RealDeal(pReq->m_jParam, pRpl->m_jData))
        {
            pRpl->m_jData = Json::Value();
        }

        pRpl->m_strRet            = pLogic->GetRetCode();
        pRpl->m_strErrorCode      = pLogic->GetErrCode();
        pRpl->m_strErrorMsg       = pLogic->GetErrMsg();
        pRpl->m_strTimestamps     = to_string(tools::GetTimeStamps());
        pRpl->m_strInvokeId       = pReq->m_strInvokeId.empty() ? tools::GetRandomString() : pReq->m_strInvokeId;
        pRpl->m_strVersion        = "1";
        pRpl->m_strCallerServerId = pReq->m_strCallerServerId.empty() ? CConfigure::GetServerId() : pReq->m_strCallerServerId;

        evrpc_request_done(pReqGeneric);
    }

private:
    virtual bool RealDeal(const Json::Value &jParam, Json::Value &jData)=0;

private:
    const string & GetRetCode() const { return m_strRetCode; }
    const string & GetErrCode() const { return m_strErrCode; }
    const string & GetErrMsg() const { return m_strErrMsg; }

protected:
    string m_strRetCode;
    string m_strErrCode;
    string m_strErrMsg;
};

#endif
