#ifndef _EXAMPLE_PROTOCOL_H_
#define _EXAMPLE_PROTOCOL_H_ 

#include <vector>
#include <boost/shared_ptr.hpp>

#include "protocol.h"

class CExampleProtocol : public IProtocol
{
public:
    CExampleProtocol() {}
    virtual ~CExampleProtocol() {}

    virtual bool Unpacking(struct evbuffer *buff, IPacketModel *packet);
    virtual bool Packets(struct evbuffer *buff, IPacketModel *packet);
};

#endif
