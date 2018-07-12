#include "example_logic.h"
#include "log.h"
#include "packet_model.h"


bool CExampleLogic::RealDeal(const Json::Value &jParam, Json::Value &jData)
{
    m_strRetCode = "0";
    m_strErrCode = "0";
    m_strErrMsg  = "success";
    jData["msg"] = "hello client";
    return true;
}
