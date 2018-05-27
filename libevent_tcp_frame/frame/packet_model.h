#ifndef _PACKET_MODEL_H_
#define _PACKET_MODEL_H_

#include <vector>

using namespace std;

class IPacketModel
{
public:
    IPacketModel() {}
    virtual ~IPacketModel() {}

    bool ReadPacket(vector<char> &vBuffer);
    bool WritePacket(vector<char> &vBuffer);
protected:
    //boost::weak_ptr<CSocketHandle> m_pSocketHandle;
};

#endif
