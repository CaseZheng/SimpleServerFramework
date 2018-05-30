#include "example_protocol.h"
#include "example_packet_model.h"

bool CExampleProtocol::Unpacking(vector<char> &buff, IPacketModel *packet)
{
    if(buff.empty())
    {
        return false;
    }
    CExampleInPacketModel *pPacketModel = dynamic_cast<CExampleInPacketModel*>(packet);
    pPacketModel->SetMsg(string(buff.begin(), buff.end()));
    buff.clear();

    return true;
}

bool CExampleProtocol::Packets(vector<char> &buff, IPacketModel *packet)
{
    CExampleOutPacketModel *pOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    buff.insert(buff.begin(), msg.begin(), msg.end());

    return true;
}
