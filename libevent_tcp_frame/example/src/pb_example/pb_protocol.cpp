#include "pb_protocol.h"
#include "pb_packet_model.h"

bool CPbProtocol::Unpacking(vector<char> &buff, IPacketModel *packet)
{
    if(buff.empty())
    {
        return false;
    }
    CPbInPacketModel *pPacketModel = dynamic_cast<CPbInPacketModel*>(packet);
    pPacketModel->SetMsg(string(buff.begin(), buff.end()));
    buff.clear();

    return true;
}

bool CPbProtocol::Packets(vector<char> &buff, IPacketModel *packet)
{
    CPbOutPacketModel *pOutPacketModel = dynamic_cast<CPbOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    buff.insert(buff.begin(), msg.begin(), msg.end());

    return true;
}
