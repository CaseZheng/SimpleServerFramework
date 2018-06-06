#include "example_protocol.h"
#include "example_packet_model.h"

#include "log.h"

#ifdef EVBUFFER
bool CExampleProtocol::Unpacking(shared_ptr<struct evbuffer> &buff, IPacketModel *packet)
{
    return true;
}
bool CExampleProtocol::Packets(shared_ptr<struct evbuffer>> &buff, IPacketModel *packet)
{
    return true;
}

#else

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
    DEBUG("=========== Packets Start ===========");
    CExampleOutPacketModel *pOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    buff.insert(buff.begin(), msg.begin(), msg.end());

    DEBUG("=========== Packets End ===========");
    return true;
}
#endif
