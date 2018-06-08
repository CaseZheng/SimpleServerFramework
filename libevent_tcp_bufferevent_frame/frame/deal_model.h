#ifndef _DEAL_MODEL_H_
#define _DEAL_MODEL_H_ 

#include <boost/shared_ptr.hpp>

#include "packet_model.h"
#include "socket_handle.h"

class CSocketHandle;
class IPacketModel;

class IDealModel
{
public:
    IDealModel() {}
    virtual ~IDealModel() {}

    virtual bool DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel) = 0;
};


#endif
