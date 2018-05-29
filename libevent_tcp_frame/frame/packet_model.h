#ifndef _PACKET_MODEL_H_
#define _PACKET_MODEL_H_

#include <boost/weak_ptr.hpp>
#include <vector>

using namespace std;

class IPacketModel
{
public:
    IPacketModel() {}
    virtual ~IPacketModel() {}
};

#endif
