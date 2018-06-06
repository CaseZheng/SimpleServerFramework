#ifndef _PB_PROTOCOL_H_
#define _PB_PROTOCOL_H_ 

#include <vector>

#include "protocol.h"

class CPbProtocol : public IProtocol
{
public:
    CPbProtocol() {}
    virtual ~CPbProtocol() {}

    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet);
    virtual bool Packets(vector<char> &buff, IPacketModel *packet);
};

#endif
