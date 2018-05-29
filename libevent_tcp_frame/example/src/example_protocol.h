#ifndef _EXAMPLE_PROTOCOL_H_
#define _EXAMPLE_PROTOCOL_H_ 

#include <vector>

#include "protocol.h"

class CExampleProtocol : public IProtocol
{
public:
    CExampleProtocol() {}
    virtual ~CExampleProtocol() {}

    virtual bool Unpacking(vector<char> &buff, IPacketModel *packet);
    virtual bool Packets(vector<char> &buff, IPacketModel *packet);
};

#endif
