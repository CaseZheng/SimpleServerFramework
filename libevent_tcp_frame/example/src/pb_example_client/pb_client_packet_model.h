#ifndef _PB_CLIENT_PACKET_MODEL_H_
#define _PB_CLIENT_PACKET_MODEL_H_

#include <vector>

#include "packet_model.h"

#include "basic_protocol.pb.h"

using namespace std;

using namespace basic_protocol;

class CPbClientInPacketModel : public IPacketModel, public PbBasicPackage
{
public:
    CPbClientInPacketModel();
    virtual ~CPbClientInPacketModel() {}
};

class CPbClientOutPacketModel : public IPacketModel, public PbBasicPackage
{
public:
    CPbClientOutPacketModel();
    virtual ~CPbClientOutPacketModel() {}
};


#endif
