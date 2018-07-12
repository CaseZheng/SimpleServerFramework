#ifndef _EXAMPLE_LOGIC_H_
#define _EXAMPLE_LOGIC_H_

#include <json/json.h>
#include "deal_model.h"

class CExampleLogic : public CLogic
{
public:
    virtual bool RealDeal(const Json::Value &jParam, Json::Value &jData);
};

#endif
