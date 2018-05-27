#ifndef _EXAMPE_PACKET_MODEL_H_
#define _EXAMPE_PACKET_MODEL_H_

#include "packet_model.h"
#include <vector>

using namespace std;

class CExampePacketModel : public IPacketModel 
{
public:
    CExampePacketModel() {}
    virtual ~CExampePacketModel() {}
    bool ReadPacket(vector<char> &vBuffer);
    bool WritePacket(vector<char> &vBuffer);
};

#endif
