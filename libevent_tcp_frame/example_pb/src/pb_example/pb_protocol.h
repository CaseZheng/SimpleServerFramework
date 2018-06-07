#ifndef _PB_PROTOCOL_H_
#define _PB_PROTOCOL_H_ 

#include <vector>

#include "protocol.h"

class CPbProtocol : public IProtocol
{
public:
    CPbProtocol() {}
    virtual ~CPbProtocol() {}

#ifdef EVBUFFER
    virtual bool Unpacking(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet);
    virtual bool Packets(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet);
#else 
    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet);
    virtual bool Packets(vector<char> &buff, IPacketModel *packet);
#endif
};

#endif
