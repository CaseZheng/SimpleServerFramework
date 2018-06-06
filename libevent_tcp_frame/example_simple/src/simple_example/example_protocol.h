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

#ifdef EVBUFFER
    virtual bool Unpacking(shared_ptr<struct evbuffer> &buff, IPacketModel *packet);
    virtual bool Packets(shared_ptr<struct evbuffer>> &buff, IPacketModel *packet);
#else
    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet);
    virtual bool Packets(vector<char> &buff, IPacketModel *packet);
#endif

};

#endif
