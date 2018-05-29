#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_ 

#include <vector>

#include "packet_model.h"

class IProtocol
{
public:
    IProtocol() {}
    virtual ~IProtocol() {}
    
    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet) = 0;
    virtual bool Packets(vector<char> &buff, IPacketModel *packet) = 0;
};

#endif
