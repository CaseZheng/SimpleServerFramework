#include <sstream>
#include "log.h"
#include "packet_model.h"

bool IPacketModel::ReadPacket(vector<char> &vBuffer)
{
    stringstream ss;
    for(auto &it : vBuffer)
    {
        ss << it;
    }
    DEBUG(ss.str());
    vBuffer.clear();
    return true;
}

bool IPacketModel::WritePacket(vector<char> &vBuffer)
{
    return true;
}
