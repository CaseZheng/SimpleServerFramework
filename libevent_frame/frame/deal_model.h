#ifndef _DEAL_MODEL_H_
#define _DEAL_MODEL_H_ 

#include <string>
#include <json/json.h>

using namespace std;

#define SET_RET_INFO(ret, retcode, retinfo) \
    {\
        strRet = (ret); \
        strRetCode = (retcode); \
        strRetInfo = (retinfo); \
    }

class CDealModel 
{
public:
    CDealModel() {}
    virtual ~CDealModel() {}

    virtual bool RealDeal(
            const Json::Value &jParams,
            string &strRet, 
            string &strRetCode, 
            string &strRetInfo,
            Json::Value &jData)=0;

private:
    CDealModel(const CDealModel&);
    CDealModel & operator=(const CDealModel&);
};

#endif
