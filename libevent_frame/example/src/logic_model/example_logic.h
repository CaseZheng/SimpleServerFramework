#ifndef _EXAMPLE_LOGIC_H_
#define _EXAMPLE_LOGIC_H_

#include <json/json.h>
#include "deal_model.h"

class CExampleLogic : public CDealModel 
{
public:
    CExampleLogic() {}
    virtual ~CExampleLogic() {}

    virtual bool RealDeal(
            const Json::Value &jParams,
            string &strRet, 
            string &strRetCode, 
            string &strRetInfo,
            Json::Value &jData);
};

#endif
