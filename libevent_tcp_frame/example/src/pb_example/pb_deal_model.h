#ifndef _PB_DEAL_MODEL_H_
#define _PB_DEAL_MODEL_H_ 

#include <boost/shared_ptr.hpp>

#include "deal_model.h"

class CPbDealModel : public IDealModel 
{
public:
    CPbDealModel() {}
    virtual ~CPbDealModel() {}

    virtual bool DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel);
};

#endif
