#ifndef _EXAMPLE_DEAL_MODEL_H_
#define _EXAMPLE_DEAL_MODEL_H_ 

#include <boost/shared_ptr.hpp>

#include "deal_model.h"

class CExampleDealModel : public IDealModel 
{
public:
    CExampleDealModel() {}
    virtual ~CExampleDealModel() {}

    virtual bool DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel);
};

class CClientExampleDealModel : public IDealModel 
{
public:
    CClientExampleDealModel() {}
    virtual ~CClientExampleDealModel() {}

    virtual bool DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel);
};

#endif
