#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_ 

#include <vector>
#include <boost/shared_ptr.hpp>

#include "packet_model.h"

class IProtocol
{
public:
    IProtocol() {}
    virtual ~IProtocol() {}
    
    virtual bool Unpacking(struct evbuffer *buff, IPacketModel *packet) = 0;
    virtual bool Packets(struct evbuffer *buff, IPacketModel *packet) = 0;
};

#endif
