#include "example_logic.h"

bool CExampleLogic::RealDeal(
            const Json::Value &jParams,
            string &strRet, 
            string &strRetCode, 
            string &strRetInfo,
            Json::Value &jData)
{
    SET_RET_INFO("0", "0", "成功");

    jData["info"] = "hello example";
    return true;
}

