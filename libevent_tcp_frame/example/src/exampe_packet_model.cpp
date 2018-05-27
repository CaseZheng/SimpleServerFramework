#include <sstream>

#include "log.h"
#include "exampe_packet_model.h"

bool CExampePacketModel::ReadPacket(vector<char> &vBuffer)
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

bool CExampePacketModel::WritePacket(vector<char> &vBuffer)
{
    return true;
}
