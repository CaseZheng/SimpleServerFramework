#ifndef _PB_PACKET_MODEL_H_
#define _PB_PACKET_MODEL_H_

#include <vector>

#include "packet_model.h"

#include "basic_protocol.pb.h"

using namespace std;

using namespace basic_protocol;

class CPbInPacketModel : public IPacketModel, public PbBasicPackage
{
public:
    CPbInPacketModel() {}
    virtual ~CPbInPacketModel() {}
};

class CPbOutPacketModel : public IPacketModel, public PbBasicPackage
{
public:
    CPbOutPacketModel() {}
    virtual ~CPbOutPacketModel() {}
};


#endif
