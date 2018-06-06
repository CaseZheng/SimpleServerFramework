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
    
#ifdef EVBUFFER
    virtual bool Unpacking(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet) = 0;
    virtual bool Packets(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet) = 0;
#else 
    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet) = 0;
    virtual bool Packets(vector<char> &buff, IPacketModel *packet) = 0;
#endif
};

#endif
